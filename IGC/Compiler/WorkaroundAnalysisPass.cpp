/*===================== begin_copyright_notice ==================================

Copyright (c) 2017 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


======================= end_copyright_notice ==================================*/

#include "Compiler/WorkaroundAnalysisPass.h"
#include "GenISAIntrinsics/GenIntrinsicInst.h"
#include "GenISAIntrinsics/GenIntrinsics.h"
#include "Compiler/IGCPassSupport.h"
#include "Compiler/MetaDataApi/IGCMetaDataDefs.h"

#include "common/LLVMWarningsPush.hpp"
#include <llvm/Support/CommandLine.h>
#include <llvm/IR/Function.h>
#include <llvm/ADT/SmallVector.h>
#include "common/LLVMWarningsPop.hpp"

#include "Compiler/CISACodeGen/helper.h"
#include "Compiler/CodeGenPublicEnums.h"
#include "Compiler/CodeGenPublic.h"

using namespace llvm;
using namespace IGC;
using namespace IGC::IGCMD;

static cl::opt<bool> EnableFMaxFMinPlusZero(
    "enable-fmax-fmin-plus-zero", cl::init(false), cl::Hidden,
    cl::desc("Enable fmax/fmin + 0.0f flag"));

// Register pass to igc-opt
#define PASS_FLAG "igc-workaround"
#define PASS_DESCRIPTION "Workaround pass used to fix functionality of special cases"
#define PASS_CFG_ONLY false
#define PASS_ANALYSIS false
IGC_INITIALIZE_PASS_BEGIN(WorkaroundAnalysis, PASS_FLAG, PASS_DESCRIPTION, PASS_CFG_ONLY, PASS_ANALYSIS)
IGC_INITIALIZE_PASS_DEPENDENCY(MetaDataUtilsWrapper)
IGC_INITIALIZE_PASS_DEPENDENCY(CodeGenContextWrapper)
IGC_INITIALIZE_PASS_END(WorkaroundAnalysis, PASS_FLAG, PASS_DESCRIPTION, PASS_CFG_ONLY, PASS_ANALYSIS)
#undef PASS_ANALYSIS
#undef PASS_CFG_ONLY
#undef PASS_DESCRIPTION
#undef PASS_FLAG

char WorkaroundAnalysis::ID = 0;

int GetSampleCResourceIdx(llvm::CallInst &I)
{
    int textLocation = -1;
    if (SampleIntrinsic* pSamplerLoadInst = dyn_cast<SampleIntrinsic>(&I))
    {
        textLocation = pSamplerLoadInst->getSamplerIndex();
        llvm::Value* pArgLocation = pSamplerLoadInst->getOperand(textLocation);
        if (pArgLocation->getType()->isPointerTy())
        {
            uint as = pArgLocation->getType()->getPointerAddressSpace();
            uint bufferIndex;
            bool directIdx;
            DecodeAS4GFXResource(as, directIdx, bufferIndex);
            return bufferIndex;
        }
    }
    return textLocation;
}

WorkaroundAnalysis::WorkaroundAnalysis()
    : FunctionPass(ID)
{
    initializeWorkaroundAnalysisPass(*PassRegistry::getPassRegistry());
}

bool WorkaroundAnalysis::runOnFunction(Function &F)
{
    m_pCtxWrapper = &getAnalysis<CodeGenContextWrapper>();
    m_pDataLayout = &F.getParent()->getDataLayout();

    llvm::IGCIRBuilder<> builder(F.getContext());
    m_builder = &builder;
    m_pModule = F.getParent();
    visit(F);
    return true;
}

const unsigned MaxTrackingDepth = 8;

/// IsKnownNotSNaN() - Check whether a value won't be an SNaN. By definition,
/// all Gen instructions on floating pointer values (except FMAX/FMIN, MOV,
/// LOAD) will return QNaN is one of the operand is NaN (either QNaN or SNaN).
static bool IsKnownNotSNaN(Value *V, unsigned Depth = 0) {
    // Is unknown if the maximal depth reaches.
    if (Depth > MaxTrackingDepth)
        return false;

    // SNaN is only possible for floating point values.
    if (!V->getType()->isFloatingPointTy())
        return true;

    // With FP constant, check whether it's SNaN directly.
    if (ConstantFP *CFP = dyn_cast<ConstantFP>(V)) {
        APFloat FVal = CFP->getValueAPF();

        if (!FVal.isNaN())
            return true;

        APInt IVal = FVal.bitcastToAPInt();
        switch (IVal.getBitWidth()) {
        default:
            break;
        case 16: return !IVal[9];
        case 32: return !IVal[22];
        case 64: return !IVal[51];
        }
        return false;
    }

    Instruction *I = dyn_cast<Instruction>(V);
    if (!I)
        return false;

    // Phi-node is known not SNaN if all its incoming values are not SNaN.
    if (PHINode *PN = dyn_cast<PHINode>(V)) {
        bool NotSNaN = true;
        for (unsigned i = 0, e = PN->getNumIncomingValues(); i != e; ++i) {
            Value *InVal = PN->getIncomingValue(i);
            if (V == InVal)
                continue;
            NotSNaN = NotSNaN && IsKnownNotSNaN(InVal, Depth + 1);
        }
        return NotSNaN;
    }

    switch (I->getOpcode()) {
    case Instruction::BitCast:
        // SNaN is possible returned after BitCast.
        return false;
    case Instruction::FSub:
        // TODO: Revisit later after unsafe math is considered during source
        // modifier folding in pattern match and/or signed zeros are telled in
        // safe math mode.
        // Skip 0 - x, which may be lowered as '-' source modifier, which won't
        // quietize the NaN.
        if (ConstantFP *CFP = dyn_cast<ConstantFP>(I->getOperand(0))) {
            if (CFP->isZero())
                return false;
        }
        return true;
    case Instruction::Select:
        return IsKnownNotSNaN(I->getOperand(1), Depth + 1) &&
               IsKnownNotSNaN(I->getOperand(2), Depth + 1);
    case Instruction::FAdd:
    case Instruction::FMul:
    case Instruction::FDiv:
        // TODO: List all instructions finally lowered into Gen insts quietize
        // the NaN.
        return true;
    case Instruction::Call:
        if (GenIntrinsicInst *GII = dyn_cast<GenIntrinsicInst>(I)) {
            switch (GII->getIntrinsicID()) {
            case GenISAIntrinsic::GenISA_rsq:
                return true;
            default:
                break;
            }
        } else if (IntrinsicInst *II = dyn_cast<IntrinsicInst>(I)) {
            switch (II->getIntrinsicID()) {
            case Intrinsic::sqrt:
            case Intrinsic::powi:
            case Intrinsic::sin:
            case Intrinsic::cos:
            case Intrinsic::pow:
            case Intrinsic::log:
            case Intrinsic::log10:
            case Intrinsic::log2:
            case Intrinsic::exp:
            case Intrinsic::exp2:
            case Intrinsic::floor:
            case Intrinsic::ceil:
            case Intrinsic::trunc:
            case Intrinsic::rint:
            case Intrinsic::nearbyint:
            case Intrinsic::fma:
            case Intrinsic::fmuladd:
            case Intrinsic::maxnum:
            case Intrinsic::minnum:
                // TODO: Do we need to add various conversions to FP?
                // NOTE: fabs since it may be folded as a source modifier.
                // TODO: Revisit fabs later after unsafe math mode is
                // considered during source modifier folding in pattern match.
                return true;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }

    return false;
}

static Constant *getQNaN(Type *Ty) {
  APFloat QNaN = APFloat::getQNaN(Ty->getFltSemantics());
  return ConstantFP::get(Ty->getContext(), QNaN);
}

void WorkaroundAnalysis::visitCallInst(llvm::CallInst &I)
{
    CodeGenContext* pCodeGenCtx = m_pCtxWrapper->getCodeGenContext();

    // TODO: Fix this for all Shaders once and for all
    if (pCodeGenCtx->type == ShaderType::VERTEX_SHADER && pCodeGenCtx->isPOSH())
    {
        if (const GenIntrinsicInst* intr = dyn_cast<GenIntrinsicInst>(&I))
        {
            VertexShaderContext* pShaderCtx = static_cast <VertexShaderContext*>(pCodeGenCtx);
            switch (intr->getIntrinsicID())
            {
                case llvm::GenISAIntrinsic::GenISA_gather4Cptr:
                case llvm::GenISAIntrinsic::GenISA_gather4POCptr:
                case llvm::GenISAIntrinsic::GenISA_gather4POptr:
                case llvm::GenISAIntrinsic::GenISA_gather4ptr:
                case llvm::GenISAIntrinsic::GenISA_sampleptr:
                case llvm::GenISAIntrinsic::GenISA_sampleLptr:
                case llvm::GenISAIntrinsic::GenISA_sampleBCptr:
                case llvm::GenISAIntrinsic::GenISA_sampleBptr:
                case llvm::GenISAIntrinsic::GenISA_sampleCptr:
                case llvm::GenISAIntrinsic::GenISA_sampleDCptr:
                case llvm::GenISAIntrinsic::GenISA_sampleDptr:
                case llvm::GenISAIntrinsic::GenISA_sampleKillPix:
                case llvm::GenISAIntrinsic::GenISA_sampleLCptr:
                case llvm::GenISAIntrinsic::GenISA_sampleinfoptr:
                    pShaderCtx->programOutput.m_SamplerCount++;
                    break;
                default:
                    break;
            }
        }
    }


    if (const GenIntrinsicInst* intr = dyn_cast<GenIntrinsicInst>(&I))
    {
        switch (intr->getIntrinsicID())
        {
        case llvm::GenISAIntrinsic::GenISA_gather4POCptr:
            GatherOffsetWorkaround(cast<SamplerGatherIntrinsic>(&I));
            break;
        case GenISAIntrinsic::GenISA_ldmsptr:
            ldmsOffsetWorkaournd(cast<LdMSIntrinsic>(&I));
            break;
        case llvm::GenISAIntrinsic::GenISA_sampleBCptr:
        case llvm::GenISAIntrinsic::GenISA_sampleCptr:
        case llvm::GenISAIntrinsic::GenISA_sampleDCptr:
        case llvm::GenISAIntrinsic::GenISA_sampleLCptr:
        {

            uint bufferIndex = GetSampleCResourceIdx(I);
            if (bufferIndex == -1) break;

            if (pCodeGenCtx->type == ShaderType::PIXEL_SHADER)
            {
                PixelShaderContext* pShaderCtx = static_cast <PixelShaderContext*>(pCodeGenCtx);
                pShaderCtx->programOutput.m_AccessedBySampleC[bufferIndex / 32] |= BIT(bufferIndex % 32);
            }
            else if (pCodeGenCtx->type == ShaderType::VERTEX_SHADER)
            {
                VertexShaderContext* pShaderCtx = static_cast <VertexShaderContext*>(pCodeGenCtx);
                pShaderCtx->programOutput.m_AccessedBySampleC[bufferIndex / 32] |= BIT(bufferIndex % 32);
            }
            else if (pCodeGenCtx->type == ShaderType::GEOMETRY_SHADER)
            {
                GeometryShaderContext* pShaderCtx = static_cast <GeometryShaderContext*>(pCodeGenCtx);
                pShaderCtx->programOutput.m_AccessedBySampleC[bufferIndex / 32] |= BIT(bufferIndex % 32);
            }
            else if (pCodeGenCtx->type == ShaderType::HULL_SHADER)
            {
                HullShaderContext* pShaderCtx = static_cast <HullShaderContext*>(pCodeGenCtx);
                pShaderCtx->programOutput.m_AccessedBySampleC[bufferIndex / 32] |= BIT(bufferIndex % 32);
            }
            else if (pCodeGenCtx->type == ShaderType::DOMAIN_SHADER)
            {
                DomainShaderContext* pShaderCtx = static_cast <DomainShaderContext*>(pCodeGenCtx);
                pShaderCtx->programOutput.m_AccessedBySampleC[bufferIndex / 32] |= BIT(bufferIndex % 32);
            }
            else if (pCodeGenCtx->type == ShaderType::COMPUTE_SHADER)
            {
                ComputeShaderContext* pShaderCtx = static_cast <ComputeShaderContext*>(pCodeGenCtx);
                pShaderCtx->programOutput.m_AccessedBySampleC[bufferIndex / 32] |= BIT(bufferIndex % 32);
            }
        }
         break;
        case llvm::GenISAIntrinsic::GenISA_RenderTargetReadSampleFreq:
        {
            //Render target read should return 0 when the sample is outside primitive processed. 
            //    R0.xyzw = RTRead(RTi, SampleIndex);
            //    R1 = 1<<SamplexIndex
            //    R2 = R1 & InputCoverage
            //    F0 = Cmp ne R2, 0
            //    Dst.w = (F0) Sel R0.w, 1
            CodeGenContext* pCodeGenCtx = m_pCtxWrapper->getCodeGenContext();
            if (pCodeGenCtx->platform.WaReturnZeroforRTReadOutsidePrimitive())
            {
                Value* one = llvm::ConstantFP::get(m_builder->getFloatTy(), 1.0);
                llvm::Instruction* cloneinst = I.clone();
                cloneinst->insertAfter(&I);
                m_builder->SetInsertPoint(&(*std::next(BasicBlock::iterator(cloneinst))));
                Function* SGV = GenISAIntrinsic::getDeclaration(
                    m_pModule,
                    GenISAIntrinsic::GenISA_DCL_SystemValue,
                    m_builder->getFloatTy());
                Value* inputCoverageFloat = m_builder->CreateCall(SGV, m_builder->getInt32(INPUT_COVERAGE_MASK));
                Value* inputCoverage = m_builder->CreateBitCast(inputCoverageFloat, m_builder->getInt32Ty());

                Value* shiftLeft = m_builder->CreateShl(m_builder->getInt32(1), I.getOperand(1));
                Value* andWithInputCoverage = m_builder->CreateAnd(shiftLeft, inputCoverage);
                Value* cmpInst = m_builder->CreateICmpNE(andWithInputCoverage, m_builder->getInt32(0));

                Value* valueX = m_builder->CreateExtractElement(cloneinst, m_builder->getInt32(0));
                Value* valueY = m_builder->CreateExtractElement(cloneinst, m_builder->getInt32(1));
                Value* valueZ = m_builder->CreateExtractElement(cloneinst, m_builder->getInt32(2));
                Value* valueW = m_builder->CreateExtractElement(cloneinst, m_builder->getInt32(3));
                
                Value* selW = m_builder->CreateSelect(cmpInst, valueW, one);

                llvm::Value* newValue = llvm::UndefValue::get(llvm::VectorType::get(m_builder->getFloatTy(), 4));
                newValue = m_builder->CreateInsertElement(newValue, valueX, m_builder->getInt32(0));
                newValue = m_builder->CreateInsertElement(newValue, valueY, m_builder->getInt32(1));
                newValue = m_builder->CreateInsertElement(newValue, valueZ, m_builder->getInt32(2));
                newValue = m_builder->CreateInsertElement(newValue, selW, m_builder->getInt32(3));

                (&I)->replaceAllUsesWith(newValue);
                I.eraseFromParent();
            }
        }
        break;
        default:
            break;
        }
    }
}

void WorkaroundAnalysis::ldmsOffsetWorkaournd(LdMSIntrinsic* ldms)
{
    // In some cases immediate offsets are not working in hardware for ldms message
    // to solve it we add directly the offset to the integer coordinate
    Value* zero = m_builder->getInt32(0); 
    if(ldms->getImmOffset(0) == zero &&
        ldms->getImmOffset(1) == zero &&
        ldms->getImmOffset(2) == zero)
    {
        return;
    }
    for(unsigned int i = 0; i < 2; i++)
    {
        m_builder->SetInsertPoint(ldms);
        Value* coord = ldms->getCoordinate(i);
        Value* newCoord = m_builder->CreateAdd(
            coord, 
            m_builder->CreateTrunc(ldms->getImmOffset(i), coord->getType()));
        ldms->setCoordinate(i, newCoord);
        ldms->setImmOffset(i, m_builder->getInt32(0));
    }
}

/// transform gather4poc into gather4
void WorkaroundAnalysis::GatherOffsetWorkaround(SamplerGatherIntrinsic* gatherpoc)
{
    if(IGC_IS_FLAG_DISABLED(EnableGather4cpoWA))
    {
        return;
    }
    Value* zero = m_builder->getInt32(0);
    if(gatherpoc->getOperand(8) != zero ||
        gatherpoc->getOperand(9) != zero ||
        gatherpoc->getOperand(10) != zero)
    {
        // only apply the WA if all the immediate offsets are zero
        return;
    }
    Value* resource = gatherpoc->getTextureValue();
    Value* sampler = gatherpoc->getSamplerValue();
    Function* resInfo = 
        GenISAIntrinsic::getDeclaration(m_pModule, GenISAIntrinsic::GenISA_resinfoptr, resource->getType());
    m_builder->SetInsertPoint(gatherpoc);
    Value* info = m_builder->CreateCall2(resInfo, resource, m_builder->getInt32(0));
    Value* arg[] =
    {
        gatherpoc->getOperand(0), // ref
        nullptr,                  // u
        nullptr,                  // v
        gatherpoc->getOperand(5), // r
        ConstantFP::get(gatherpoc->getOperand(0)->getType(), 0.0),   // ai
        resource,
        sampler,
        zero,
        zero,
        zero,
        gatherpoc->getOperand(11),
    };
    for(unsigned int i = 0; i < 2; i++)
    {
        Value* coord = gatherpoc->getOperand(i + 1);
        Value* offset = gatherpoc->getOperand(i + 3);

        Value* size = m_builder->CreateExtractElement(info, m_builder->getInt32(i));
        size = m_builder->CreateUIToFP(size, coord->getType());
        Value* invSize = m_builder->CreateFDiv(ConstantFP::get(coord->getType(), 1.0), size);

        // offset is only encoded on 6 bits
        offset = m_builder->CreateShl(offset, m_builder->getInt32(32 - 6));
        offset = m_builder->CreateAShr(offset, m_builder->getInt32(32 - 6));
        offset = m_builder->CreateSIToFP(offset, coord->getType());
        //
        Value* newCoord = m_builder->CreateFMul(offset, invSize);
        newCoord = m_builder->CreateFAdd(newCoord, coord);
        arg[i + 1] = newCoord;
    }
    Type* types[] =
    {
        gatherpoc->getType(),
        gatherpoc->getOperand(0)->getType(),
        resource->getType(),
        sampler->getType(),
    }; 
    Function* gather4cFunc =
        GenISAIntrinsic::getDeclaration(m_pModule, GenISAIntrinsic::GenISA_gather4Cptr, types);
    Value* gather4c = m_builder->CreateCall(gather4cFunc, arg);
    gatherpoc->replaceAllUsesWith(gather4c);
    gatherpoc->eraseFromParent();
}


#define PASS_FLAG "igc-wa-fminmax"
#define PASS_DESCRIPTION "Workaround fmax/fmin"
#define PASS_CFG_ONLY false
#define PASS_ANALYSIS false
IGC_INITIALIZE_PASS_BEGIN(WAFMinFMax, PASS_FLAG, PASS_DESCRIPTION, PASS_CFG_ONLY, PASS_ANALYSIS)
IGC_INITIALIZE_PASS_DEPENDENCY(MetaDataUtilsWrapper)
IGC_INITIALIZE_PASS_DEPENDENCY(CodeGenContextWrapper)
IGC_INITIALIZE_PASS_END(WAFMinFMax, PASS_FLAG, PASS_DESCRIPTION, PASS_CFG_ONLY, PASS_ANALYSIS)

char WAFMinFMax::ID = 0;

WAFMinFMax::WAFMinFMax()
    : FunctionPass(ID)
{
    initializeWAFMinFMaxPass(*PassRegistry::getPassRegistry());
}

bool WAFMinFMax::runOnFunction(Function& F)
{
    m_ctx = getAnalysis<CodeGenContextWrapper>().getCodeGenContext();

    llvm::IGCIRBuilder<> builder(F.getContext());
    m_builder = &builder;
    visit(F);
    return true;
}

void WAFMinFMax::visitCallInst(CallInst &I)
{
    if (const IntrinsicInst* intr = dyn_cast<IntrinsicInst>(&I))
    {
        switch (intr->getIntrinsicID())
        {
        case Intrinsic::maxnum:
        case Intrinsic::minnum:
        {
            if (m_ctx->m_DriverInfo.SupportsIEEEMinMax())
            {
                // OCL's fmax/fmin maps to GEN's max/min in non-IEEE mode.
                // By default, gen uses non-IEEE mode.  But, in BDW and SKL
                // prior to C step, IEEE mode is used if denorm bit is set.
                // If there are no sNaN as inputs to fmax/fmin,  IEEE mode
                // is the same as non-IEEE mode;  if there are sNaN,  non-IEEE
                // mode is NOT the same as IEEE mode. But non-IEEE mode is the
                // same as the following
                //     non-IEEE_fmax(x, y) =
                //           x1 = IEEE_fmin(x, qNaN), y1 = IEEE_fmin(y, qNaN)
                //             (or fadd(x, -0.0); y1 = fadd(y, -0.0);)
                //           IEEE_fmax(x1, y1)
                // SKL C+ has IEEE minmax bit in Control Register(CR), so far we
                // don't set it (meaning non-ieee mode).
                //
                // Therefore, if fmax/fmin is in IEEE mode, we need to workaround
                // that by converting sNAN to qNAN if one of operands is sNAN but
                // needing to preserve the original value if it's not sNAN.
                //
                // There are more than one ways to achieve that:
                //  - X + 0.0
                //    It's the simplest one. However, it cannot preserver -0.0
                //    as -0.0 + 0.0 = 0.0. It also has other issues depending
                //    on rounding mode. We could enhance it by adding -0.0 if X
                //    is negative. But that
                //    introduces additional overhead.
                //
                //  The following two are both good candidates with single
                //  instruction overhead only:
                //
                //  - x * 1.0
                //  - FMIN(x, qNAN) or FMAX(x, qNAN)
                //
                //    According to PRM, both of them should aways give x or
                //    qNAN.
                //
                // We choose FMIN to prevent other optimizations kicking in.
                //

                // Note that m_enableFMaxFMinPlusZero is used here for GEN9 only; if it
                // is set,  it means that IEEE-mode min/max is used if denorm bit is set.
                Type *Ty = intr->getType();
                ModuleMetaData *modMD = getAnalysis<MetaDataUtilsWrapper>().getModuleMetaData();
                bool minmaxModeSetByDenormBit =
                    (!m_ctx->platform.hasIEEEMinmaxBit() ||
                        m_ctx->platform.WaOCLEnableFMaxFMinPlusZero() ||
                        EnableFMaxFMinPlusZero);
                bool hasNaNs = !modMD->compOpt.FiniteMathOnly;
                if (hasNaNs && minmaxModeSetByDenormBit &&
                    ((Ty->isFloatTy() && (m_ctx->m_floatDenormMode32 == FLOAT_DENORM_RETAIN)) ||
                    (Ty->isDoubleTy() && (m_ctx->m_floatDenormMode64 == FLOAT_DENORM_RETAIN)) ||
                        (Ty->isHalfTy() && (m_ctx->m_floatDenormMode16 == FLOAT_DENORM_RETAIN))))
                {
                    m_builder->SetInsertPoint(&I);

                    Intrinsic::ID IID = Intrinsic::minnum;
                    Function *IFunc =
                        Intrinsic::getDeclaration(I.getParent()->getParent()->getParent(),
                            IID, I.getType());
                    Value *QNaN = getQNaN(I.getType());

                    Value* src0 = I.getOperand(0);
                    if (!IsKnownNotSNaN(src0))
                        I.setArgOperand(0, m_builder->CreateCall2(IFunc, src0, QNaN));

                    Value* src1 = I.getOperand(1);
                    if (!IsKnownNotSNaN(src1))
                        I.setArgOperand(1, m_builder->CreateCall2(IFunc, src1, QNaN));
                }
            }
            break;
        }

        default:
            break;
        }
    }
}