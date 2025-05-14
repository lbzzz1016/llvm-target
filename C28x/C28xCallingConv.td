//==- C28xCallingConv.td - Calling Conventions for MSP430 -*- tablegen -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for C28x architecture.
//===----------------------------------------------------------------------===//
def CSR_Base : CalleeSavedRegs<(add XAR1, XAR2, XAR3)>;

// TODO should save R4H-R7H on FPU32, R4L-R7L on FPU64
// TODO structure and union should be handled
// Argument calling convention
def CC_C28x_Call : CallingConv<[
  // Promote i8 to i16
  CCIfType<[i8], CCPromoteToType<i16>>,

  // Dump arguments to registers based on their length
  CCIfType<[i16], CCAssignToReg<[AL, AH]>>,
  CCIfType<[i32], CCAssignToReg<[ACC]>>,

]>;

def CC_C28x_Ret : CallingConv<[

  CCIfType<[i8], CCPromoteToType<i16>>,

  CCIfType<[i16], CCAssignToReg<[AL]>>,
  CCIfType<[i32], CCAssignToReg<[ACC]>>,
]>;