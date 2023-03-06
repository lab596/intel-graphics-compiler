/*========================== begin_copyright_notice ============================

Copyright (C) 2017-2023 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/
#ifndef IGA_MODELS_GEN8_HPP
#define IGA_MODELS_GEN8_HPP
// clang-format off

// ******************** DO NOT MODIFY DIRECTLY ********************
// Auto-generated by IGA project tools

#include "../Models.hpp"

namespace iga {
static const iga::OpSpec MODEL_GEN8_OPSPECS[unsigned(Op::TOTAL_OPS) + 1] {
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::ADD /* Op::1 */, Platform::GEN8, 0x40,
    "add",
    "Addition",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // F <- UB,B,UW,W,UD,D
      {TYPE(Type::F),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // DF <- DF
      {TYPE(Type::DF),TYPE(Type::DF)},
      // HF <- HF
      {TYPE(Type::HF),TYPE(Type::HF)},
      // HF <- UB,B,UW,W,UD,D
      {TYPE(Type::HF),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)},
      // HF,F <- HF,F
      {TYPE(Type::HF)|TYPE(Type::F),TYPE(Type::HF)|TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::ADDC /* Op::3 */, Platform::GEN8, 0x4E,
    "addc",
    "Addition with Carry",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER
  },
  {Op::AND /* Op::4 */, Platform::GEN8, 0x05,
    "and",
    "Logic And",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)}
    },
    OpSpec::Attr::IS_BITWISE|OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::ASR /* Op::5 */, Platform::GEN8, 0x0C,
    "asr",
    "Arithmetic Shift Right",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)}
    },
    OpSpec::Attr::IS_BITWISE|OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::AVG /* Op::6 */, Platform::GEN8, 0x42,
    "avg",
    "Average",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::BFE /* Op::7 */, Platform::GEN8, 0x18,
    "bfe",
    "Bit Field Extract",
    OpSpec::Format::TERNARY_REGIMM_REG_REGIMM,
    {
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)},
      // D <- D
      {TYPE(Type::D),TYPE(Type::D)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::BFI1 /* Op::8 */, Platform::GEN8, 0x19,
    "bfi1",
    "Bit Field Insert 1",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)},
      // D <- D
      {TYPE(Type::D),TYPE(Type::D)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::BFI2 /* Op::9 */, Platform::GEN8, 0x1A,
    "bfi2",
    "Bit Field Insert 2",
    OpSpec::Format::TERNARY_REGIMM_REG_REGIMM,
    {
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)},
      // D <- D
      {TYPE(Type::D),TYPE(Type::D)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::BFREV /* Op::11 */, Platform::GEN8, 0x17,
    "bfrev",
    "Bit Field Reverse",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::BRC /* Op::12 */, Platform::GEN8, 0x23,
    "brc",
    "Branch Converging",
    OpSpec::Format::JUMP_BINARY_BRC,
    {
      //  <- D
      {ENUM_BITSET_EMPTY_VALUE,TYPE(Type::D)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::BRD /* Op::13 */, Platform::GEN8, 0x21,
    "brd",
    "Branch Diverging",
    OpSpec::Format::JUMP_UNARY_REGIMM,
    {
      //  <- D
      {ENUM_BITSET_EMPTY_VALUE,TYPE(Type::D)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::BREAK /* Op::14 */, Platform::GEN8, 0x28,
    "break",
    "Break",
    OpSpec::Format::JUMP_BINARY_IMM_IMM,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::CALL /* Op::15 */, Platform::GEN8, 0x2C,
    "call",
    "Call",
    OpSpec::Format::JUMP_UNARY_CALL_REGIMM,
    {
      // D,UD <-
      {TYPE(Type::D)|TYPE(Type::UD),ENUM_BITSET_EMPTY_VALUE}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::CALLA /* Op::16 */, Platform::GEN8, 0x2B,
    "calla",
    "Call Absolute",
    OpSpec::Format::JUMP_UNARY_CALL_REGIMM,
    {
      // D,UD <-
      {TYPE(Type::D)|TYPE(Type::UD),ENUM_BITSET_EMPTY_VALUE}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::CBIT /* Op::17 */, Platform::GEN8, 0x4D,
    "cbit",
    "Count Bits Set",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // UD <- UB,UW,UD
      {TYPE(Type::UD),TYPE(Type::UB)|TYPE(Type::UW)|TYPE(Type::UD)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::CMP /* Op::18 */, Platform::GEN8, 0x10,
    "cmp",
    "Compare",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // F <- UB,B,UW,W,UD,D
      {TYPE(Type::F),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // DF <- DF
      {TYPE(Type::DF),TYPE(Type::DF)},
      // HF <- HF
      {TYPE(Type::HF),TYPE(Type::HF)},
      // HF <- UB,B,UW,W,UD,D
      {TYPE(Type::HF),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)},
      // HF,F <- HF,F
      {TYPE(Type::HF)|TYPE(Type::F),TYPE(Type::HF)|TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::CMPN /* Op::19 */, Platform::GEN8, 0x11,
    "cmpn",
    "Compare NaN",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // F <- UB,B,UW,W,UD,D
      {TYPE(Type::F),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // DF <- DF
      {TYPE(Type::DF),TYPE(Type::DF)},
      // HF <- HF
      {TYPE(Type::HF),TYPE(Type::HF)},
      // F,HF <- F,HF
      {TYPE(Type::F)|TYPE(Type::HF),TYPE(Type::F)|TYPE(Type::HF)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::CONT /* Op::20 */, Platform::GEN8, 0x29,
    "cont",
    "Continue",
    OpSpec::Format::JUMP_BINARY_IMM_IMM,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::CSEL /* Op::21 */, Platform::GEN8, 0x12,
    "csel",
    "Conditional Select",
    OpSpec::Format::TERNARY_REGIMM_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::DP2 /* Op::23 */, Platform::GEN8, 0x57,
    "dp2",
    "Dot Product 2",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::DP3 /* Op::24 */, Platform::GEN8, 0x56,
    "dp3",
    "Dot Product 3",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::DP4 /* Op::25 */, Platform::GEN8, 0x54,
    "dp4",
    "Dot Product 4",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::DPH /* Op::29 */, Platform::GEN8, 0x55,
    "dph",
    "Dot Product Homogeneous",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::ELSE /* Op::30 */, Platform::GEN8, 0x24,
    "else",
    "Else",
    OpSpec::Format::JUMP_BINARY_IMM_IMM,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_BRCTL
  },
  {Op::ENDIF /* Op::31 */, Platform::GEN8, 0x25,
    "endif",
    "End If",
    OpSpec::Format::JUMP_UNARY_IMM,
    {}, // no type mappings
    OpSpec::Attr::NONE
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::FBH /* Op::34 */, Platform::GEN8, 0x4B,
    "fbh",
    "Find First Bit from MSB Side",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // UD <- D,UD
      {TYPE(Type::UD),TYPE(Type::D)|TYPE(Type::UD)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::FBL /* Op::35 */, Platform::GEN8, 0x4C,
    "fbl",
    "Find First Bit from LSB Side",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::FRC /* Op::36 */, Platform::GEN8, 0x43,
    "frc",
    "Fraction",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::GOTO /* Op::37 */, Platform::GEN8, 0x2E,
    "goto",
    "Goto",
    OpSpec::Format::JUMP_BINARY_IMM_IMM,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_BRCTL|OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::HALT /* Op::38 */, Platform::GEN8, 0x2A,
    "halt",
    "Halt",
    OpSpec::Format::JUMP_BINARY_IMM_IMM,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::IF /* Op::39 */, Platform::GEN8, 0x22,
    "if",
    "If",
    OpSpec::Format::JUMP_BINARY_IMM_IMM,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_BRCTL|OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::ILLEGAL /* Op::40 */, Platform::GEN8, 0x00,
    "illegal",
    "Illegal",
    OpSpec::Format::NULLARY,
    {}, // no type mappings
    OpSpec::Attr::NONE
  },
  {Op::JMPI /* Op::41 */, Platform::GEN8, 0x20,
    "jmpi",
    "Jump Indexed",
    OpSpec::Format::JUMP_UNARY_REGIMM,
    {
      //  <- D
      {ENUM_BITSET_EMPTY_VALUE,TYPE(Type::D)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::JOIN /* Op::42 */, Platform::GEN8, 0x2F,
    "join",
    "Join",
    OpSpec::Format::JUMP_UNARY_IMM,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::LINE /* Op::43 */, Platform::GEN8, 0x59,
    "line",
    "Line",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::LRP /* Op::44 */, Platform::GEN8, 0x5C,
    "lrp",
    "Linear Interpolation",
    OpSpec::Format::TERNARY_REGIMM_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::LZD /* Op::45 */, Platform::GEN8, 0x4A,
    "lzd",
    "Leading Zero Detection",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // UD <- D,UD
      {TYPE(Type::UD),TYPE(Type::D)|TYPE(Type::UD)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::MAC /* Op::46 */, Platform::GEN8, 0x48,
    "mac",
    "Multiply Accumulate",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)},
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // DF <- DF
      {TYPE(Type::DF),TYPE(Type::DF)},
      // HF <- HF
      {TYPE(Type::HF),TYPE(Type::HF)},
      // HF,F <- HF,F
      {TYPE(Type::HF)|TYPE(Type::F),TYPE(Type::HF)|TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::MACH /* Op::47 */, Platform::GEN8, 0x49,
    "mach",
    "Multiply Accumulate High",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // D <- D
      {TYPE(Type::D),TYPE(Type::D)},
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::MAD /* Op::49 */, Platform::GEN8, 0x5B,
    "mad",
    "Multiply Add",
    OpSpec::Format::TERNARY_REGIMM_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // DF <- DF
      {TYPE(Type::DF),TYPE(Type::DF)},
      // HF <- HF
      {TYPE(Type::HF),TYPE(Type::HF)},
      // HF,F <- HF,F
      {TYPE(Type::HF)|TYPE(Type::F),TYPE(Type::HF)|TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::MADM /* Op::50 */, Platform::GEN8, 0x5D,
    "madm",
    "Multiply Add for Macro",
    OpSpec::Format::TERNARY_REGIMM_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // DF <- DF
      {TYPE(Type::DF),TYPE(Type::DF)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::MATH /* Op::51 */, Platform::GEN8, 0x38,
    "math",
    "Extended Math Function",
    OpSpec::Format::MATH_BINARY_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // D <- D
      {TYPE(Type::D),TYPE(Type::D)},
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)},
      // F,HF <- F,HF
      {TYPE(Type::F)|TYPE(Type::HF),TYPE(Type::F)|TYPE(Type::HF)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::MOV /* Op::52 */, Platform::GEN8, 0x01,
    "mov",
    "Move",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // F <- UB,B,UW,W,UD,D
      {TYPE(Type::F),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UB,B,UW,W,UD,D <- F
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::F)},
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // DF <- UW,W,UD,D
      {TYPE(Type::DF),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // DF <- F
      {TYPE(Type::DF),TYPE(Type::F)},
      // UW,W,UD,D <- DF
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::DF)},
      // F <- DF
      {TYPE(Type::F),TYPE(Type::DF)},
      // DF <- DF
      {TYPE(Type::DF),TYPE(Type::DF)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- F
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::F)},
      // UQ,Q <- DF
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::DF)},
      // F <- UQ,Q
      {TYPE(Type::F),TYPE(Type::UQ)|TYPE(Type::Q)},
      // DF <- UQ,Q
      {TYPE(Type::DF),TYPE(Type::UQ)|TYPE(Type::Q)},
      // HF <- UB,B,UW,W,UD,D
      {TYPE(Type::HF),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // HF <- F
      {TYPE(Type::HF),TYPE(Type::F)},
      // UB,B,UW,W,UD,D <- HF
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::HF)},
      // F <- HF
      {TYPE(Type::F),TYPE(Type::HF)},
      // HF <- HF
      {TYPE(Type::HF),TYPE(Type::HF)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::MOVI /* Op::53 */, Platform::GEN8, 0x03,
    "movi",
    "Move Indexed",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // B <- B
      {TYPE(Type::B),TYPE(Type::B)},
      // UB <- UB
      {TYPE(Type::UB),TYPE(Type::UB)},
      // W <- W
      {TYPE(Type::W),TYPE(Type::W)},
      // UW <- UW
      {TYPE(Type::UW),TYPE(Type::UW)},
      // D <- D
      {TYPE(Type::D),TYPE(Type::D)},
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)},
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::MUL /* Op::54 */, Platform::GEN8, 0x41,
    "mul",
    "Multiply",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B <- UB,B
      {TYPE(Type::UB)|TYPE(Type::B),TYPE(Type::UB)|TYPE(Type::B)},
      // UW,W <- UB,B
      {TYPE(Type::UW)|TYPE(Type::W),TYPE(Type::UB)|TYPE(Type::B)},
      // UD,D <- UB,B
      {TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)},
      // UW,W <- UW,W
      {TYPE(Type::UW)|TYPE(Type::W),TYPE(Type::UW)|TYPE(Type::W)},
      // UD,D <- UW,W
      {TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)},
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // DF <- DF
      {TYPE(Type::DF),TYPE(Type::DF)},
      // HF <- HF
      {TYPE(Type::HF),TYPE(Type::HF)},
      // HF,F <- HF,F
      {TYPE(Type::HF)|TYPE(Type::F),TYPE(Type::HF)|TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::NOP /* Op::56 */, Platform::GEN8, 0x7E,
    "nop",
    "No Operation",
    OpSpec::Format::NULLARY,
    {}, // no type mappings
    OpSpec::Attr::NONE
  },
  {Op::NOT /* Op::57 */, Platform::GEN8, 0x04,
    "not",
    "Logic Not",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)}
    },
    OpSpec::Attr::IS_BITWISE|OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::OR /* Op::58 */, Platform::GEN8, 0x06,
    "or",
    "Logic Or",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)}
    },
    OpSpec::Attr::IS_BITWISE|OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::PLN /* Op::59 */, Platform::GEN8, 0x5A,
    "pln",
    "Plane",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION
  },
  {Op::RET /* Op::60 */, Platform::GEN8, 0x2D,
    "ret",
    "Return",
    OpSpec::Format::JUMP_UNARY_REG,
    {
      //  <- D,UD
      {ENUM_BITSET_EMPTY_VALUE,TYPE(Type::D)|TYPE(Type::UD)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::RNDD /* Op::61 */, Platform::GEN8, 0x45,
    "rndd",
    "Round Down",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::RNDE /* Op::62 */, Platform::GEN8, 0x46,
    "rnde",
    "Round to Nearest or Even",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::RNDU /* Op::63 */, Platform::GEN8, 0x44,
    "rndu",
    "Round Up",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::RNDZ /* Op::64 */, Platform::GEN8, 0x47,
    "rndz",
    "Round to Zero",
    OpSpec::Format::BASIC_UNARY_REGIMM,
    {
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::SAD2 /* Op::67 */, Platform::GEN8, 0x50,
    "sad2",
    "Sum of Absolute Difference 2",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // W,UW <- B,UB
      {TYPE(Type::W)|TYPE(Type::UW),TYPE(Type::B)|TYPE(Type::UB)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::SADA2 /* Op::68 */, Platform::GEN8, 0x51,
    "sada2",
    "Sum of Absolute Difference Accumulate 2",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // W,UW <- B,UB
      {TYPE(Type::W)|TYPE(Type::UW),TYPE(Type::B)|TYPE(Type::UB)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::SEL /* Op::70 */, Platform::GEN8, 0x02,
    "sel",
    "Select",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // F <- F
      {TYPE(Type::F),TYPE(Type::F)},
      // DF <- DF
      {TYPE(Type::DF),TYPE(Type::DF)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)},
      // HF <- HF
      {TYPE(Type::HF),TYPE(Type::HF)},
      // HF,F <- HF,F
      {TYPE(Type::HF)|TYPE(Type::F),TYPE(Type::HF)|TYPE(Type::F)}
    },
    OpSpec::Attr::IS_SELECT|OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::SEND /* Op::71 */, Platform::GEN8, 0x31,
    "send",
    "Send Message",
    OpSpec::Format::SEND_UNARY,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::SENDC /* Op::72 */, Platform::GEN8, 0x32,
    "sendc",
    "Conditional Send Message",
    OpSpec::Format::SEND_UNARY,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::SHL /* Op::77 */, Platform::GEN8, 0x09,
    "shl",
    "Shift Left",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)}
    },
    OpSpec::Attr::IS_BITWISE|OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::SHR /* Op::78 */, Platform::GEN8, 0x08,
    "shr",
    "Shift Right",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,UW,UD <- UB,UW,UD
      {TYPE(Type::UB)|TYPE(Type::UW)|TYPE(Type::UD),TYPE(Type::UB)|TYPE(Type::UW)|TYPE(Type::UD)},
      // UW,UD <- UW,UD
      {TYPE(Type::UW)|TYPE(Type::UD),TYPE(Type::UW)|TYPE(Type::UD)},
      // UQ <- UW,UD
      {TYPE(Type::UQ),TYPE(Type::UW)|TYPE(Type::UD)},
      // UW,UD <- UQ
      {TYPE(Type::UW)|TYPE(Type::UD),TYPE(Type::UQ)},
      // UQ <- UQ
      {TYPE(Type::UQ),TYPE(Type::UQ)}
    },
    OpSpec::Attr::IS_BITWISE|OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION|OpSpec::Attr::SUPPORTS_SRCMODS
  },
  {Op::SMOV /* Op::79 */, Platform::GEN8, 0x0A,
    "smov",
    "Scattered Move",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UW,W,UD,D,UQ,Q,F,DF <- UW,W,UD,D,UQ,Q,F,DF
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)|TYPE(Type::UQ)|TYPE(Type::Q)|TYPE(Type::F)|TYPE(Type::DF),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)|TYPE(Type::UQ)|TYPE(Type::Q)|TYPE(Type::F)|TYPE(Type::DF)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::SUBB /* Op::81 */, Platform::GEN8, 0x4F,
    "subb",
    "Integer Subtraction with Borrow",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)}
    },
    OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SATURATION
  },
  {Op::INVALID, Platform::GEN8, 0x0, nullptr, nullptr, OpSpec::Format::INVALID, {}, OpSpec::Attr::NONE, },
  {Op::WAIT /* Op::83 */, Platform::GEN8, 0x30,
    "wait",
    "Wait Notification",
    OpSpec::Format::SYNC_UNARY,
    {
      // UD <- UD
      {TYPE(Type::UD),TYPE(Type::UD)}
    },
    OpSpec::Attr::NONE
  },
  {Op::WHILE /* Op::84 */, Platform::GEN8, 0x27,
    "while",
    "While",
    OpSpec::Format::JUMP_UNARY_IMM,
    {}, // no type mappings
    OpSpec::Attr::SUPPORTS_PREDICATION
  },
  {Op::XOR /* Op::85 */, Platform::GEN8, 0x07,
    "xor",
    "Logic Xor",
    OpSpec::Format::BASIC_BINARY_REG_REGIMM,
    {
      // UB,B,UW,W,UD,D <- UB,B,UW,W,UD,D
      {TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UB)|TYPE(Type::B)|TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UW,W,UD,D
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UQ,Q <- UW,W,UD,D
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D)},
      // UW,W,UD,D <- UQ,Q
      {TYPE(Type::UW)|TYPE(Type::W)|TYPE(Type::UD)|TYPE(Type::D),TYPE(Type::UQ)|TYPE(Type::Q)},
      // UQ,Q <- UQ,Q
      {TYPE(Type::UQ)|TYPE(Type::Q),TYPE(Type::UQ)|TYPE(Type::Q)}
    },
    OpSpec::Attr::IS_BITWISE|OpSpec::Attr::SUPPORTS_PREDICATION|OpSpec::Attr::SUPPORTS_FLAGMODIFIER|OpSpec::Attr::SUPPORTS_SRCMODS
  }
}; // end MODEL_GEN8_OPSPECS
} // namespace iga
// clang-format on
#endif // IGA_MODELS_GEN8_HPP
