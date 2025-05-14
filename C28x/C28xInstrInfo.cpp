//===-- C28xInstrInfo.cpp - C28x instruction information ------*- c++ -*-===//
//
// part of the llvm project, under the apache license v2.0 with llvm exceptions.
// see https://llvm.org/license.txt for license information.
// spdx-license-identifier: apache-2.0 with llvm-exception
//
//===----------------------------------------------------------------------===//
//
// this file contains the C28x implementation of the targetinstrinfo class.
//
//===----------------------------------------------------------------------===//

#include "C28xInstrInfo.h"
#include "C28x.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "C28xGenInstrInfo.inc"

// Pin the vtable to this file
void C28xInstrInfo::anchor() {}

C28xInstrInfo::C28xInstrInfo(C28xSubtarget &STI) : RI() {}

unsigned C28xInstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
  switch (MI.getOpcode()) {
    default:
      return MI.getDesc().getSize();
  }

}
