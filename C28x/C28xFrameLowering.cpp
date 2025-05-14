//===-- C28xFrameLowering.cpp - C28x Frame Information ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the C28x implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "C28xFrameLowering.h"
#include "C28xSubtarget.h"

using namespace llvm;

C28xFrameLowering::C28xFrameLowering(const C28xSubtarget &STI)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown,
                          Align(2), 0,  Align(2),
                          false),
      STI(STI), TRI(STI.getRegisterInfo()) {}

bool C28xFrameLowering::hasFP(const MachineFunction &MF) const {
  return false;
}

void C28xFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  ;
}

void C28xFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  ;
}