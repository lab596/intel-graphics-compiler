/*========================== begin_copyright_notice ============================

Copyright (C) 2017-2023 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/
#ifndef IGA_BXML_OPS_HPP
#define IGA_BXML_OPS_HPP
// clang-format off

// ******************** DO NOT MODIFY DIRECTLY ********************
// Auto-generated by IGA project tools

namespace iga
{
enum class Op
{
  INVALID = 0,
  FIRST_OP = 1,

  ADD          = FIRST_OP +   0,
  ADD3         = FIRST_OP +   1,
  ADDC         = FIRST_OP +   2,
  AND          = FIRST_OP +   3,
  ASR          = FIRST_OP +   4,
  AVG          = FIRST_OP +   5,
  BFE          = FIRST_OP +   6,
  BFI1         = FIRST_OP +   7,
  BFI2         = FIRST_OP +   8,
  BFN          = FIRST_OP +   9,
  BFREV        = FIRST_OP +  10,
  BRC          = FIRST_OP +  11,
  BRD          = FIRST_OP +  12,
  BREAK        = FIRST_OP +  13,
  CALL         = FIRST_OP +  14,
  CALLA        = FIRST_OP +  15,
  CBIT         = FIRST_OP +  16,
  CMP          = FIRST_OP +  17,
  CMPN         = FIRST_OP +  18,
  CONT         = FIRST_OP +  19,
  CSEL         = FIRST_OP +  20,
  DIM          = FIRST_OP +  21,
  DP2          = FIRST_OP +  22,
  DP3          = FIRST_OP +  23,
  DP4          = FIRST_OP +  24,
  DP4A         = FIRST_OP +  25,
  DPAS         = FIRST_OP +  26,
  DPASW        = FIRST_OP +  27,
  DPH          = FIRST_OP +  28,
  ELSE         = FIRST_OP +  29,
  ENDIF        = FIRST_OP +  30,
  F16TO32      = FIRST_OP +  31,
  F32TO16      = FIRST_OP +  32,
  FBH          = FIRST_OP +  33,
  FBL          = FIRST_OP +  34,
  FRC          = FIRST_OP +  35,
  GOTO         = FIRST_OP +  36,
  HALT         = FIRST_OP +  37,
  IF           = FIRST_OP +  38,
  ILLEGAL      = FIRST_OP +  39,
  JMPI         = FIRST_OP +  40,
  JOIN         = FIRST_OP +  41,
  LINE         = FIRST_OP +  42,
  LRP          = FIRST_OP +  43,
  LZD          = FIRST_OP +  44,
  MAC          = FIRST_OP +  45,
  MACH         = FIRST_OP +  46,
  MACL         = FIRST_OP +  47,
  MAD          = FIRST_OP +  48,
  MADM         = FIRST_OP +  49,
  MATH         = FIRST_OP +  50,
  MOV          = FIRST_OP +  51,
  MOVI         = FIRST_OP +  52,
  MUL          = FIRST_OP +  53,
  NOP          = FIRST_OP +  55,
  NOT          = FIRST_OP +  56,
  OR           = FIRST_OP +  57,
  PLN          = FIRST_OP +  58,
  RET          = FIRST_OP +  59,
  RNDD         = FIRST_OP +  60,
  RNDE         = FIRST_OP +  61,
  RNDU         = FIRST_OP +  62,
  RNDZ         = FIRST_OP +  63,
  ROL          = FIRST_OP +  64,
  ROR          = FIRST_OP +  65,
  SAD2         = FIRST_OP +  66,
  SADA2        = FIRST_OP +  67,
  SEL          = FIRST_OP +  69,
  SEND         = FIRST_OP +  70,
  SENDC        = FIRST_OP +  71,
  SENDS        = FIRST_OP +  74,
  SENDSC       = FIRST_OP +  75,
  SHL          = FIRST_OP +  76,
  SHR          = FIRST_OP +  77,
  SMOV         = FIRST_OP +  78,
  SRND         = FIRST_OP +  79,
  SUBB         = FIRST_OP +  80,
  SYNC         = FIRST_OP +  81,
  WAIT         = FIRST_OP +  82,
  WHILE        = FIRST_OP +  83,
  XOR          = FIRST_OP +  84,

  LAST_OP   = FIRST_OP + 84,
  TOTAL_OPS = LAST_OP
};
} // iga::
// clang-format on
#endif // IGA_BXML_OPS_HPP
