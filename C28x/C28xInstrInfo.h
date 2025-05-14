//===-- C28xInstrInfo.h - C28x Instruction Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the C28x implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_C28X_C28XINSTRINFO_H
#define LLVM_LIB_TARGET_C28X_C28XINSTRINFO_H

#include "C28xRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "C28xGenInstrInfo.inc"

namespace llvm {
class C28xSubtarget;

// InstrInfo needs RegisterInfo
// RegisterInfo needs FrameLowering
// FrameLowering needs InstrInfo
class C28xInstrInfo : public C28xGenInstrInfo {
  const C28xRegisterInfo RI;
  virtual void anchor();

public:
  explicit C28xInstrInfo(C28xSubtarget &STI);

  // Whenever a client has an instance of instruction info, it should always be
  // able to get register info as well
  const C28xRegisterInfo &getRegisterInfo() const { return RI; }

  unsigned getInstSizeInBytes(const MachineInstr &MI) const override;
};

} // name space llvm
#endif
