//===-- C28xRegisterInfo.h - C28x Register Information Impl -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the C28x implementation of the MRegisterInfo class.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_C28X_C28XREGISTERINFO_H
#define LLVM_LIB_TARGET_C28X_C28XREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "C28xGenRegisterInfo.inc"

namespace llvm {

class C28xRegisterInfo : public C28xGenRegisterInfo {
public:
  C28xRegisterInfo();

  // Required by CodeGen
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;
  BitVector getReservedRegs(const MachineFunction &MF) const override;
  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  // Required by DebugInfo
  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif
