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

#pragma once

#include "cif/common/id.h"

namespace IGC {

namespace CodeType {
using CodeType_t = uint64_t;
using CodeTypeCoder = CIF::Coder<CodeType_t>;
constexpr auto llvmLl = CodeTypeCoder::Enc("LLVM_LL");
constexpr auto llvmBc = CodeTypeCoder::Enc("LLVM_BC");
constexpr auto spirV = CodeTypeCoder::Enc("SPIRV");
constexpr auto oclC = CodeTypeCoder::Enc("OCL_C");
constexpr auto oclCpp = CodeTypeCoder::Enc("OCL_CPP");
constexpr auto oclGenBin = CodeTypeCoder::Enc("OCL_GEN_BIN");
constexpr auto elf = CodeTypeCoder::Enc("ELF");
constexpr auto undefined = CodeTypeCoder::Enc("UNDEFINED");
constexpr auto invalid = CodeTypeCoder::Enc("INVALID");
}

}
