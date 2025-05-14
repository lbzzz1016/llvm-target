//==- MSP430FrameLowering.h - Define frame lowering for MSP430 --*- C++ -*--==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_C28X_C28XFRAMELOWERING_H
#define LLVM_LIB_TARGET_C28X_C28XFRAMELOWERING_H

#include "C28x.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class C28xSubtarget;
class C28xRegisterInfo;
// This class is required by C28xGenRegisterInfo
class C28xFrameLowering : public TargetFrameLowering {
protected:
public:
  C28xFrameLowering(const C28xSubtarget &STI);
  const C28xSubtarget &STI;
  const C28xRegisterInfo *TRI;

  bool hasFP(const MachineFunction &MF) const override;

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

};

} // namespace llvm
#endif
