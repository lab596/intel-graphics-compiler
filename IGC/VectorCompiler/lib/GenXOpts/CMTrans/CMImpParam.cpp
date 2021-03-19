/*========================== begin_copyright_notice ============================

Copyright (c) 2000-2021 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

============================= end_copyright_notice ===========================*/

//===----------------------------------------------------------------------===//
//
/// CMImpParam
/// ----------
///
/// As well as explicit kernel args declared in the CM kernel function, certain
/// implicit args are also passed. These fall into 3 categories:
///
/// 1. fields set up in r0 by the hardware, depending on which dispatch method
///    is being used (e.g. media walker);
///
/// 2. implicit args set up along with the explicit args in CURBE by the CM
///    runtime.
///
/// 3. implicit OCL/L0 args set up, e.g. private base, byval arg linearization.
///
/// The r0 implicit args are represented in LLVM IR by special intrinsics, and the
/// GenX backend generates these to special reserved vISA registers.
///
/// For the CM runtime implicit args in (2) above, in vISA 3.2 and earlier, these were also
/// represented by LLVM special intrinsics and vISA special reserved vISA registers.
/// Because they are specific to the CM runtime, and not any other user of vISA,
/// vISA 3.3 has removed them, and instead they are handled much like other kernel
/// args in the input table.
///
/// The *kind* byte in the input table has two fields:
///
/// * the *category* field, saying whether the input is general/surface/etc;
///
/// * the *provenance* field, saying whether the input is an explicit one from
///   the CM source, or an implicit one generated by this pass. This is a
///   protocol agreed between the CM compiler (in fact this pass) and the CM
///   runtime.
///
/// Within the CM compiler, the vISA input table for a kernel is represented by an
/// array of kind bytes, each one corresponding to an argument of the kernel function.
///
/// Clang codegen still generates special intrinsics for these CM runtime implicit
/// args. It is the job of this CMImpParam pass to transform those intrinsics:
///
/// * where the intrinsic for a CM runtime implicit arg is used somewhere:
///
///   - a global variable is created for it;
///
///   - for any kernel that uses the implicit arg (or can reach a subroutine that
///     uses it), the implicit arg is added to the input table in the kernel
///     metadata and as an extra arg to the definition of the kernel itself,
///     and its value is stored into the global variable;
///
/// * each use of the intrinsic for a CM runtime implicit arg is transformed into
///   a load of the corresponding global variable.
///
/// Like any other global variable, the subsequent CMABI pass turns the global
/// variable for an implicit arg into local variable(s) passed into subroutines
/// if necessary.
///
/// This pass also linearizes kernel byval arguments.
/// If a kernel has an input pointer argument with byval attribute, it means
/// that it will be passed as a value with the argument's size = sizeof(the
/// type), not sizeof(the type *). To support such kinds of arguments, VC (as
/// well as scalar IGC) makes implicit linearization, e.g.
///
///   %struct.s1 = type { [2 x i32], i8 } ===> i32, i32, i8
///
/// This implicit linearization is added as kernel arguments and mapped via
/// metadata to the original explicit byval argument.
///
///   %struct.s1 = type { [2 x i32], i8 }
///
///   declare i32 @foo(%struct.s1* byval(%struct.s1) "VCArgumentDesc"="svmptr_t"
///                    "VCArgumentIOKind"="0" "VCArgumentKind"="0" %arg, i64
///                    %arg1);
///
/// Will be transformed into (byval args uses will be changed in
/// CMKernelArgOffset)
///
///   declare i32 @foo(%struct.s1* byval(%struct.s1) "VCArgumentDesc"="svmptr_t"
///                     "VCArgumentIOKind"="0" "VCArgumentKind"="0" %arg, i64
///                     %arg1, i32 %__arg_lin__arg_0, i32 %__arg_lin__arg_1, i8
///                     %__arg_lin__arg_2);
///
/// Additionally, information about these implicit linearization will be written
/// to kernel metadata as internal::KernelMDOp::LinearizationArgs. It stores
/// mapping between explicit byval argument and its linearization.
///
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "cmimpparam"
#include "vc/GenXOpts/GenXOpts.h"
#include "vc/GenXOpts/Utils/KernelInfo.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/GenXIntrinsics/GenXIntrinsics.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Module.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include <set>
#include <stack>
#include <map>
#include "Probe/Assertion.h"

#include "llvmWrapper/IR/DerivedTypes.h"
#include "llvmWrapper/IR/Function.h"

using namespace llvm;

static cl::opt<bool>
    CMRTOpt("cmimpparam-cmrt", cl::init(true), cl::Hidden,
            cl::desc("Should be used only in llvm opt to switch RT"));

namespace {

class ImplicitUseInfo {
public:
  typedef std::set<unsigned> ImplicitSetTy;

  explicit ImplicitUseInfo(Function *F) : Fn(F) {}
  ImplicitUseInfo() : Fn(nullptr) {}

  Function *getFunction() const { return Fn; }

  bool empty() const { return Implicits.empty(); }
  ImplicitSetTy &getImplicits() { return Implicits; }
  const ImplicitSetTy &getImplicits() const { return Implicits; }

  // \brief Add an implicit arg intrinsic
  void addImplicit(unsigned IID) { Implicits.insert(IID); }

  void merge(const ImplicitUseInfo &IUI) {
    Implicits.insert(IUI.Implicits.begin(), IUI.Implicits.end());
  }

  void dump() const { print(dbgs()); }

  void print(raw_ostream &OS, unsigned depth = 0) const {
    for (auto IID : Implicits) {
      OS.indent(depth) << GenXIntrinsic::getAnyName(IID, None) << "\n";
    }
  }

private:
  // \brief The function being analyzed
  Function *Fn;

  // \brief Implicit arguments used
  ImplicitSetTy Implicits;
};

// Helper struct to store temporary information for implicit arguments
// linearization.
struct LinearizationElt {
  Type *Ty;
  unsigned Offset;
};
using LinearizedTy = std::vector<LinearizationElt>;
using ArgLinearization = std::unordered_map<Argument *, LinearizedTy>;

struct CMImpParam : public ModulePass {
  static char ID;
  bool IsCmRT;
  const DataLayout *DL = nullptr;

  CMImpParam(bool isCmRT = true) : ModulePass(ID), IsCmRT(isCmRT && CMRTOpt) {
    initializeCMImpParamPass(*PassRegistry::getPassRegistry());
  }

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<CallGraphWrapperPass>();
  }

  virtual StringRef getPassName() const { return "CM Implicit Params"; }

  virtual bool runOnModule(Module &M);

  void dump() const { print(dbgs()); }
  virtual void print(raw_ostream &OS, const Module *M = nullptr) const;

private:
  void replaceWithGlobal(CallInst *CI, unsigned IID);
  bool AnalyzeImplicitUse(Module &M);
  void WriteArgsLinearizationInfo(Module &M);

  LinearizedTy LinearizeAggregateType(Type *AggrTy);
  ArgLinearization GenerateArgsLinearizationInfo(Function &F);

  void MergeImplicits(ImplicitUseInfo &implicits, Function *F);
  void PropagateImplicits(Function *F, Module &M,
                          ImplicitUseInfo &implicits);
  CallGraphNode *ProcessKernel(Function *F);

  static Value *getValue(Metadata *M) {
    if (auto VM = dyn_cast<ValueAsMetadata>(M))
      return VM->getValue();
    return nullptr;
  }

  // Convert to implicit thread payload related intrinsics.
  bool ConvertToOCLPayload(Module &M);

  uint32_t MapToKind(unsigned IID) {
    using namespace genx;
    switch (IID) {
      default:
        return KernelMetadata::AK_NORMAL;
      case GenXIntrinsic::genx_print_buffer:
        return KernelMetadata::AK_NORMAL | KernelMetadata::IMP_OCL_PRINTF_BUFFER;
      case GenXIntrinsic::genx_local_size:
        return KernelMetadata::AK_NORMAL | KernelMetadata::IMP_LOCAL_SIZE;
      case GenXIntrinsic::genx_local_id:
      case GenXIntrinsic::genx_local_id16:
        return KernelMetadata::AK_NORMAL | KernelMetadata::IMP_LOCAL_ID;
      case GenXIntrinsic::genx_group_count:
        return KernelMetadata::AK_NORMAL | KernelMetadata::IMP_GROUP_COUNT;
      case GenXIntrinsic::genx_get_scoreboard_deltas:
        return KernelMetadata::AK_NORMAL | KernelMetadata::IMP_SB_DELTAS;
      case GenXIntrinsic::genx_get_scoreboard_bti:
        return KernelMetadata::AK_SURFACE | KernelMetadata::IMP_SB_BTI;
      case GenXIntrinsic::genx_get_scoreboard_depcnt:
        return KernelMetadata::AK_SURFACE | KernelMetadata::IMP_SB_DEPCNT;
      case GenXIntrinsic::genx_local_id_x:
        return KernelMetadata::AK_NORMAL | KernelMetadata::IMP_OCL_LOCAL_ID_X;
      case GenXIntrinsic::genx_local_id_y:
        return KernelMetadata::AK_NORMAL | KernelMetadata::IMP_OCL_LOCAL_ID_Y;
      case GenXIntrinsic::genx_local_id_z:
        return KernelMetadata::AK_NORMAL | KernelMetadata::IMP_OCL_LOCAL_ID_Z;
      case GenXIntrinsic::genx_group_or_local_size:
        return KernelMetadata::AK_NORMAL |
               KernelMetadata::IMP_OCL_GROUP_OR_LOCAL_SIZE;
    }
    return KernelMetadata::AK_NORMAL;
  }

  // \brief Returns the implicit use info associated with a function
  ImplicitUseInfo &getImplicitUseInfo(Function *F) {
    if (!ImplicitsInfo.count(F)) {
      ImplicitUseInfo *IUI = new ImplicitUseInfo(F);
      ImplicitsInfoObjs.push_back(IUI);
      ImplicitsInfo[F] = IUI;
      return *IUI;
    }
    return *ImplicitsInfo[F];
  }

  // \brief Returns the implict use info associated with a function (kernel)
  // and also creates a new one that represents the total implicits for the
  // kernel as a whole (stored in a different object)
  ImplicitUseInfo &getImplicitUseInfoKernel(Function *F) {
    IGC_ASSERT(Kernels.count(F));

    if (KernelsInfo.count(F)) {
      // Kernel already processed
      return *KernelsInfo[F];
    }

    ImplicitUseInfo *IUI = new ImplicitUseInfo(F);
    ImplicitsInfoObjs.push_back(IUI);
    KernelsInfo[F] = IUI;

    if (ImplicitsInfo.count(F)) {
      IUI->merge(*ImplicitsInfo[F]);
    }

    return *IUI;
  }

  const ImplicitUseInfo *implicitUseInfoKernelExist(Function *F) const {
    if (KernelsInfo.count(F)) {
      auto CI = KernelsInfo.find(F);
      return CI->second;
    }

    return nullptr;
  }

  void addImplicit(Function *F, unsigned IID) {
    getImplicitUseInfo(F).addImplicit(IID);
  }

  GlobalVariable *getIIDGlobal(Function *F, unsigned IID) {
    if (GlobalsMap.count(IID))
      return GlobalsMap[IID];

    Type * Ty = getIntrinRetType(F->getContext(), IID);
    IGC_ASSERT(Ty);
    GlobalVariable *NewVar = new GlobalVariable(
        *F->getParent(), Ty, false,
        GlobalVariable::InternalLinkage,
        Constant::getNullValue(Ty),
        "__imparg_" + GenXIntrinsic::getAnyName(IID, None));
    GlobalsMap[IID] = NewVar;

    return NewVar;
  }

  Type *getIntrinRetType(LLVMContext &Context, unsigned IID) {
    switch (IID) {
      case GenXIntrinsic::genx_print_buffer:
        return llvm::Type::getInt64Ty(Context);
      case GenXIntrinsic::genx_local_id:
      case GenXIntrinsic::genx_local_size:
      case GenXIntrinsic::genx_group_count:
        return IGCLLVM::FixedVectorType::get(llvm::Type::getInt32Ty(Context),
                                             3);
      case GenXIntrinsic::genx_local_id16:
        return IGCLLVM::FixedVectorType::get(llvm::Type::getInt16Ty(Context),
                                             3);
      default:
        // Should be able to extract the type from the intrinsic
        // directly as no overloading is required (if it is then
        // you need to define specific type in a case statement above)
        FunctionType *FTy = dyn_cast_or_null<FunctionType>(
                                    GenXIntrinsic::getAnyType(Context, IID));
        if (FTy)
          return FTy->getReturnType();
    }
    return nullptr;
  }

  // This map captures all implicit uses to be transformed
  SmallDenseMap<Function *, ImplicitUseInfo *> ImplicitsInfo;

  // This map captures all implicit uses that are required for a kernel
  // (includes sub function uses)
  SmallDenseMap<Function *, ImplicitUseInfo *> KernelsInfo;

  // All kernels (entry points) in module being processed
  SmallPtrSet<Function *, 8> Kernels;

  // Already visited functions
  SmallPtrSet<Function *, 8> AlreadyVisited;

  // ImplicitUseInfo objects created
  SmallVector<ImplicitUseInfo *, 8> ImplicitsInfoObjs;

  // Functions that contain implicit arg intrinsics
  SmallPtrSet<Function *, 8> ContainImplicit;

  // GlobalVariables that have been created for an intrinsic
  SmallDenseMap<unsigned, GlobalVariable *> GlobalsMap;
};

} // namespace

bool CMImpParam::runOnModule(Module &M) {
  DL = &M.getDataLayout();

  bool changed = false;

  // Apply necessary changes if kernels are compiled for OpenCL runtime.
  changed |= ConvertToOCLPayload(M);

  // Analyze functions for implicit use intrinsic invocation
  changed |= AnalyzeImplicitUse(M);

  // Collect all CM kernels from named metadata and also traverse the call graph
  // to determine what the total implicit uses are for the top level kernels
  if (NamedMDNode *Named = M.getNamedMetadata(genx::FunctionMD::GenXKernels)) {
    for (unsigned I = 0, E = Named->getNumOperands(); I != E; ++I) {
      MDNode *Node = Named->getOperand(I);
      if (auto F = dyn_cast_or_null<Function>(
              getValue(Node->getOperand(genx::KernelMDOp::FunctionRef)))) {
        genx::internal::createInternalMD(*F);
        Kernels.insert(F);
        AlreadyVisited.clear();
        ImplicitUseInfo &implicits = getImplicitUseInfoKernel(F);
        PropagateImplicits(F, M, implicits);
        // for OCL/L0 RT we should unconditionally add
        // implicit PRIVATE_BASE argument which is not supported on CM RT
        if (!implicits.empty() || !IsCmRT) {
          ProcessKernel(F);
          changed |= true;
        }
      }
    }
  }

  for (ImplicitUseInfo *Obj : ImplicitsInfoObjs)
    delete Obj;

  return changed;
}

// Replace the given instruction with a load from a global
void CMImpParam::replaceWithGlobal(CallInst *CI, unsigned IID) {
  GlobalVariable *GV = getIIDGlobal(CI->getParent()->getParent(), IID);
  LoadInst *Load =
      new LoadInst(GV->getType()->getPointerElementType(), GV,
                   GV->getName() + ".val", /* isVolatile */ false, CI);
  CI->replaceAllUsesWith(Load);
}

static bool isSupportedAggregateArgument(Argument &Arg) {
  if (!Arg.getType()->isPointerTy())
    return false;
  if (!Arg.hasByValAttr())
    return false;

  Type *Ty = Arg.getType()->getPointerElementType();
  auto *STy = cast<StructType>(Ty);
  IGC_ASSERT(!STy->isOpaque());
  return true;
}

// A helper structure to store current state of the aggregate traversal.
struct PendingTypeInfo {
  Type *Ty;         // Type to decompose
  unsigned NextElt; // Subelement number to decompose next
  unsigned Offset;  // Offset for the trivial type in Ty
};

// Byval aggregate arguments must be linearized. This function decomposes the
// aggregate type into primitive types recursively.
// Example:
//   struct s1 {
//     struct s2 {
//       int a;
//     };
//     char b;
//   };
//
//                Pending(stack) | LinTy(output)
// Start:
//                s1, 0, 0       | -
// Iteration 0:
//                s1, 1, 4       | -
//                s2, 0, 0       |
//   Comment: two elements in stack. s1, 1, 4 means subtype number 1 in the
//   s1 must be decomposed. The first trivial type in the 1 subtype of s1 will
//   have offset = 4. Note that this subtype may be also an aggregate type. In
//   this case, offset = 4 will be propagated to the first nested trivial type.
//   It is a recursive function, rewritten to use stack, so as not to have
//   recursion problems.
// Iteration 1:
//                s1, 1, 4       | -
//                int,0, 0       | -
// Iteration 2:
//                s1, 1, 4       | int, 0
// Iteration 3:
//                char, 0, 4     | int, 0
// Iteration 4:
//                -              | int, 0
//                               | char, 4
//
LinearizedTy CMImpParam::LinearizeAggregateType(Type *AggrTy) {
  LinearizedTy LinTy;

  std::stack<PendingTypeInfo> Pending;
  Pending.push({AggrTy, 0, 0});

  while (!Pending.empty()) {
    PendingTypeInfo Info = Pending.top();
    Pending.pop();
    Type *CurTy = Info.Ty;
    unsigned CurElt = Info.NextElt;
    unsigned NextElt = CurElt + 1;
    if (auto *STy = dyn_cast<StructType>(CurTy)) {
      unsigned NumElts = STy->getStructNumElements();
      const StructLayout *Layout = DL->getStructLayout(STy);

      IGC_ASSERT(CurElt < NumElts);
      Type *EltType = STy->getElementType(CurElt);
      if (NumElts > NextElt) {
        unsigned CurOffset = Layout->getElementOffset(CurElt);
        unsigned EltOffset = Layout->getElementOffset(NextElt) - CurOffset;
        Pending.push({CurTy, NextElt, Info.Offset + EltOffset});
      }
      Pending.push({EltType, 0, Info.Offset});

    } else if (auto *ATy = dyn_cast<ArrayType>(CurTy)) {
      unsigned NumElts = ATy->getNumElements();
      Type *EltTy = CurTy->getContainedType(0);
      unsigned EltSize = DL->getTypeStoreSize(EltTy);

      if (NumElts > NextElt)
        Pending.push({Info.Ty, NextElt, Info.Offset + EltSize});
      Pending.push({EltTy, 0, Info.Offset});
    } else
      LinTy.push_back({CurTy, Info.Offset});
  }

  return LinTy;
}

// For each byval aggregate calculate types of implicit args and their offsets
// in this aggregate.
ArgLinearization CMImpParam::GenerateArgsLinearizationInfo(Function &F) {
  ArgLinearization Lin;
  for (auto &Arg : F.args()) {
    if (!isSupportedAggregateArgument(Arg))
      continue;

    Type *ArgTy = Arg.getType();
    IGC_ASSERT(isa<PointerType>(ArgTy));
    auto *STy = cast<StructType>(ArgTy->getPointerElementType());
    Lin[&Arg] = LinearizeAggregateType(STy);
  }
  return Lin;
}

// For each function, see if it uses an intrinsic that in turn requires an
// implicit kernel argument
// (such as llvm.genx.local.size)
bool CMImpParam::AnalyzeImplicitUse(Module &M) {
  bool changed = false;

  for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I) {
    Function *Fn = &*I;
    LLVM_DEBUG(dbgs() << "AnalyzeImplicitUse visiting " << Fn->getName() << "\n");

    bool implicitUse = false;

    SmallVector<CallInst *, 8> ToErase;

    // FIXME I think this should scan function declarations to find the implicit
    // arg intrinsics, then scan their uses, instead of scanning the whole code
    // to find calls to them.
    for (inst_iterator II = inst_begin(Fn), IE = inst_end(Fn); II != IE; ++II) {
      Instruction *Inst = &*II;
      if (CallInst *CI = dyn_cast<CallInst>(Inst)) {
        if (Function *Callee = CI->getCalledFunction()) {
          auto IID = GenXIntrinsic::getAnyIntrinsicID(Callee);
          if (GenXIntrinsic::isAnyNonTrivialIntrinsic(IID)) {
            switch (IID) {
              case GenXIntrinsic::genx_local_size:
              case GenXIntrinsic::genx_local_id:
              case GenXIntrinsic::genx_local_id16:
              case GenXIntrinsic::genx_group_count:
              case GenXIntrinsic::genx_get_scoreboard_deltas:
              case GenXIntrinsic::genx_get_scoreboard_bti:
              case GenXIntrinsic::genx_get_scoreboard_depcnt:
              case GenXIntrinsic::genx_local_id_x:
              case GenXIntrinsic::genx_local_id_y:
              case GenXIntrinsic::genx_local_id_z:
              case GenXIntrinsic::genx_group_or_local_size:
              case GenXIntrinsic::genx_print_buffer:
                LLVM_DEBUG(dbgs() << "AnalyzeImplicitUse found "
                             << GenXIntrinsic::getGenXName((GenXIntrinsic::ID)IID, None));
                addImplicit(Fn, IID);
                implicitUse = true;

                // Replace the intrinsic with a load of a global at this point
                replaceWithGlobal(CI, IID);
                ToErase.push_back(CI);
                changed = true;
                break;
              default:
                // Ignore (default added to prevent compiler warnings)
                break;
            }
          }
        }
      }
    }

    for (auto CI : ToErase)
      CI->eraseFromParent();

    // Mark this function as containing an implicit use intrinsic
    if (implicitUse)
      ContainImplicit.insert(Fn);
  }

  return changed;
}

// Convert to implicit thread payload related intrinsics.
bool CMImpParam::ConvertToOCLPayload(Module &M) {
  // Check if this kernel is compiled for OpenCL runtime.
  bool DoConversion = false;

  if (NamedMDNode *Named = M.getNamedMetadata(genx::FunctionMD::GenXKernels)) {
    for (unsigned I = 0, E = Named->getNumOperands(); I != E; ++I) {
      MDNode *Node = Named->getOperand(I);
      auto F = dyn_cast_or_null<Function>(
          getValue(Node->getOperand(genx::KernelMDOp::FunctionRef)));
      if (F && (F->hasFnAttribute(genx::FunctionMD::OCLRuntime) || !IsCmRT)) {
        DoConversion = true;
        break;
      }
    }
  }

  if (!DoConversion)
    return false;

  bool Changed = false;
  auto getFn = [=, &M](unsigned ID, Type *Ty) {
    return M.getFunction(GenXIntrinsic::getAnyName(ID, Ty));
  };

  // Convert genx_local_id -> zext(genx_local_id16)
  Type *Ty32 =
      IGCLLVM::FixedVectorType::get(Type::getInt32Ty(M.getContext()), 3);
  Type *Ty16 =
      IGCLLVM::FixedVectorType::get(Type::getInt16Ty(M.getContext()), 3);
  if (auto LIDFn = getFn(GenXIntrinsic::genx_local_id, Ty32)) {
    Function *LID16 = GenXIntrinsic::getGenXDeclaration(
        &M, GenXIntrinsic::genx_local_id16, Ty16);
    for (auto UI = LIDFn->user_begin(); UI != LIDFn->user_end();) {
      auto UInst = dyn_cast<Instruction>(*UI++);
      if (UInst) {
        IRBuilder<> Builder(UInst);
        Value *Val = Builder.CreateCall(LID16);
        Val = Builder.CreateZExt(Val, Ty32);
        UInst->replaceAllUsesWith(Val);
        UInst->eraseFromParent();
        Changed = true;
      }
    }
  }
  return Changed;
}

// Merge implicit uses from the supplied function with implicit set passed in
void CMImpParam::MergeImplicits(ImplicitUseInfo &implicits, Function *F) {
  IGC_ASSERT_MESSAGE(ImplicitsInfo.count(F), "Function not found in implicits info map");
  auto IInfo = ImplicitsInfo[F];
  implicits.merge(*IInfo);
}

// Determine if the named function uses any functions tagged with implicit use
// in the call-graph
void CMImpParam::PropagateImplicits(Function *F, Module &M,
                                    ImplicitUseInfo &implicits) {
  // Traverse the call graph from the Kernel to determine what implicits are
  // used
  CallGraph &CG = getAnalysis<CallGraphWrapperPass>().getCallGraph();

  // If this node has already been processed then return immediately
  if (AlreadyVisited.count(F))
    return;

  // Add this node to the already visited list
  AlreadyVisited.insert(F);

  // Start the traversal
  CallGraphNode *N = CG[F];
  // Inspect all children (recursive)
  for (auto Children : *N) {
    auto Func = Children.second->getFunction();
    // Does this function have implicit arg use?
    if (ContainImplicit.count(Func)) {
      // Yes - add the implicits it contains to the set so far
      MergeImplicits(implicits, Func);
    }
    // Also recursively process children of this node
    PropagateImplicits(Func, M, implicits);
  }
}

// Process a kernel - loads from a global (and the globals) have already been
// added if required elsewhere (in doInitialization)
// We've already determined that this is a kernel and that it requires some
// implicit arguments adding
CallGraphNode *CMImpParam::ProcessKernel(Function *F) {
  LLVMContext &Context = F->getContext();

  IGC_ASSERT_MESSAGE(Kernels.count(F), "ProcessKernel invoked on non-kernel CallGraphNode");

  AttributeList AttrVec;
  const AttributeList &PAL = F->getAttributes();

  ArgLinearization ArgsLin = GenerateArgsLinearizationInfo(*F);

  // Determine the new argument list
  SmallVector<Type *, 8> ArgTys;

  // First transfer all the explicit arguments from the old kernel
  unsigned ArgIndex = 0;
  for (Function::arg_iterator I = F->arg_begin(), E = F->arg_end(); I != E;
       ++I, ++ArgIndex) {
    ArgTys.push_back(I->getType());
    AttributeSet attrs = PAL.getParamAttributes(ArgIndex);
    if (attrs.hasAttributes()) {
      AttrBuilder B(attrs);
      AttrVec = AttrVec.addParamAttributes(Context, ArgIndex, B);
    }
  }

  bool UsesImplicits = KernelsInfo.count(F) > 0;

  // Now add all the implicit arguments
  if (UsesImplicits) {
    ImplicitUseInfo *IUI = KernelsInfo[F];
    for (unsigned IID : IUI->getImplicits()) {
      ArgTys.push_back(getIntrinRetType(Context, IID));
      // TODO: Might need to also add any attributes from the intrinsic at some
      // point
    }
  }
  if (!IsCmRT) {
    // PRIVATE_BASE arg
    ArgTys.push_back(Type::getInt64Ty(F->getContext()));

    // Add types of implicit aggregates linearization
    for (const auto &ArgLin : ArgsLin) {
      for (const auto &LinTy : ArgLin.second)
        ArgTys.push_back(LinTy.Ty);
    }
  }

  FunctionType *NFTy = FunctionType::get(F->getReturnType(), ArgTys, false);
  IGC_ASSERT_MESSAGE((NFTy != F->getFunctionType()),
    "type out of sync, expect bool arguments)");

  // Add any function attributes
  AttributeSet FnAttrs = PAL.getFnAttributes();
  if (FnAttrs.hasAttributes()) {
    AttrBuilder B(FnAttrs);
    AttrVec = AttrVec.addAttributes(Context, AttributeList::FunctionIndex, B);
  }

  // Create new function body and insert into the module
  Function *NF = Function::Create(NFTy, F->getLinkage(), F->getName());
  NF->setAttributes(AttrVec);
  LLVM_DEBUG(dbgs() << "CMImpParam: Transforming to: " << *NF << "\n" << "From: "
        << *F);
  F->getParent()->getFunctionList().insert(F->getIterator(), NF);
  NF->takeName(F);
  NF->setSubprogram(F->getSubprogram()); // tranfer debug-info
  // DISubprogram must be unique to the module.
  // Since F can be left as a "hanging" entity in the module - we preserve
  // IR correctness by detaching DISubprogram node from it
  F->setSubprogram(nullptr);

  // Now to splice the body of the old function into the new function
  NF->getBasicBlockList().splice(NF->begin(), F->getBasicBlockList());

  // Loop over the argument list, transferring uses of the old arguments to the
  // new arguments, also tranferring over the names as well
  std::unordered_map<const Argument *, Argument *> OldToNewArg;
  Function::arg_iterator I2 = NF->arg_begin();
  for (Function::arg_iterator I = F->arg_begin(), E = F->arg_end(); I != E;
       ++I, ++I2) {
    I->replaceAllUsesWith(I2);
    I2->takeName(I);
    OldToNewArg[&*I] = &*I2;
  }

  // Get the insertion point ready for stores to globals
  Instruction &FirstI = *NF->getEntryBlock().begin();
  llvm::SmallVector<uint32_t, 8> ImpKinds;

  if (UsesImplicits) {
    ImplicitUseInfo *IUI = KernelsInfo[F];
    for (unsigned IID : IUI->getImplicits()) {
      // We known that for each IID implicit we've already added an arg
      // Rename the arg to something more meaningful here
      IGC_ASSERT_MESSAGE(I2 != NF->arg_end(),
        "fewer parameters for new function than expected");
      I2->setName("__arg_" + GenXIntrinsic::getAnyName(IID, None));

      // Also insert a new store at the start of the function to the global
      // variable used for this implicit argument intrinsic
      IGC_ASSERT_MESSAGE(GlobalsMap.count(IID),
        "no global associated with this imp arg intrinsic");
      new StoreInst(I2, GlobalsMap[IID], &FirstI);

      // Prepare the kinds that will go into the metadata
      ImpKinds.push_back(MapToKind(IID));

      ++I2;
    }
  }

  // Collect arguments linearization to store as metadata.
  genx::ArgToImplicitLinearization LinearizedArgs;
  if (!IsCmRT) {
    // Private base
    I2->setName("privBase");
    ImpKinds.push_back(genx::KernelMetadata::AK_NORMAL |
                       genx::KernelMetadata::IMP_OCL_PRIVATE_BASE);
    ++I2;

    for (const auto &ArgLin : ArgsLin) {
      Argument *ExplicitArg = OldToNewArg[ArgLin.first];
      genx::LinearizedArgInfo &LinearizedArg = LinearizedArgs[ExplicitArg];
      for (const auto &LinTy : ArgLin.second) {
        I2->setName("__arg_lin_" + ExplicitArg->getName() + "." +
                    std::to_string(LinTy.Offset));
        ImpKinds.push_back(genx::KernelMetadata::AK_NORMAL |
                           genx::KernelMetadata::IMP_OCL_LINEARIZATION);
        auto &Ctx = F->getContext();
        auto *I32Ty = Type::getInt32Ty(Ctx);
        ConstantInt *Offset = ConstantInt::get(I32Ty, LinTy.Offset);
        LinearizedArg.push_back({&*I2, Offset});
        ++I2;
      }
    }
  }

  CallGraph &CG = getAnalysis<CallGraphWrapperPass>().getCallGraph();
  CallGraphNode *NF_CGN = CG.getOrInsertFunction(NF);

  if (F->hasDLLExportStorageClass())
    NF->setDLLStorageClass(F->getDLLStorageClass());

  genx::replaceFunctionRefMD(*F, *NF);

  SmallVector<unsigned, 8> ArgKinds;
  genx::KernelMetadata KM(NF);
  // Update arg kinds for the NF.
  for (unsigned i = 0; i < KM.getNumArgs(); ++i) {
    if (LinearizedArgs.count(IGCLLVM::getArg(*NF, i)))
      ArgKinds.push_back(genx::KernelMetadata::AK_NORMAL |
                         genx::KernelMetadata::IMP_OCL_BYVALSVM);
    else
      ArgKinds.push_back(KM.getArgKind(i));
  }
  std::copy(ImpKinds.begin(), ImpKinds.end(), std::back_inserter(ArgKinds));
  KM.updateArgKindsMD(std::move(ArgKinds));
  KM.updateLinearizationMD(std::move(LinearizedArgs));

  // Now that the old function is dead, delete it. If there is a dangling
  // reference to the CallGraphNode, just leave the dead function around
  NF_CGN->stealCalledFunctionsFrom(CG[F]);
  CallGraphNode *CGN = CG[F];
  if (CGN->getNumReferences() == 0)
    delete CG.removeFunctionFromModule(CGN);
  else
    F->setLinkage(Function::ExternalLinkage);

  return NF_CGN;
}

void CMImpParam::print(raw_ostream &OS, const Module *M) const {
  OS << "Kernels : \n";

  for (auto Func : Kernels) {
    OS.indent(4) << Func->getName() << "\n";

    const ImplicitUseInfo *IUI = implicitUseInfoKernelExist(Func);
    if (IUI)
      IUI->print(OS, 8);
  }

  OS << "Functions with implicit arg intrinsics : \n";

  for (auto FuncInfoPair : ImplicitsInfo) {
    OS.indent(4) << FuncInfoPair.first->getName() << "\n";

    FuncInfoPair.second->print(OS, 8);
  }
}


char CMImpParam::ID = 0;
INITIALIZE_PASS_BEGIN(CMImpParam, "cmimpparam",
                      "Transformations required to support implicit arguments",
                      false, false)
INITIALIZE_PASS_DEPENDENCY(CallGraphWrapperPass)
INITIALIZE_PASS_END(CMImpParam, "cmimpparam",
                    "Transformations required to support implicit arguments",
                    false, false)

Pass *llvm::createCMImpParamPass(bool IsCMRT) { return new CMImpParam(IsCMRT); }
