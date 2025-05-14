//===-- C28xRegisterInfo.cpp - C28x Register Information --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the C28x implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "C28xRegisterInfo.h"
#include "C28xFrameLowering.h"
#include "C28xTargetMachine.h"
#include "llvm/CodeGen/MachineFunction.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "C28xGenRegisterInfo.inc"

// Constructor
C28xRegisterInfo::C28xRegisterInfo()
  : C28xGenRegisterInfo(C28x::PC) { }

const MCPhysReg *
C28xRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  // Defined by callingconv.td CSR_Base
  return CSR_Base_SaveList;
}

BitVector C28xRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(C28x::PC);
  Reserved.set(C28x::RPC);
  Reserved.set(C28x::SP);
  Reserved.set(C28x::ST0);
  Reserved.set(C28x::ST1);
  Reserved.set(C28x::IER);
  Reserved.set(C28x::IFR);

  return Reserved;
}

bool C28xRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  return false;
}


Register C28xRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return C28x::SP;
}

