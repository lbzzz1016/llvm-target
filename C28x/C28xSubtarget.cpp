//===-- C28xSubtarget.cpp - C28x Subtarget Information ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the C28x specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "C28xSubtarget.h"
#include "C28x.h"

using namespace llvm;

#define DEBUG_TYPE "c28x-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "C28xGenSubtargetInfo.inc"

static cl::opt<C28xSubtarget::FloatSupport>
FloatModelOption("float-model",
                 cl::desc("Floating point mode"),
                 cl::init(C28xSubtarget::NoFloat),
                 cl::Hidden
                 );

void C28xSubtarget::anchor() {}

C28xSubtarget &
C28xSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS) {
  if (CPU.empty())
    CPU = "c28x";

  ParseSubtargetFeatures(CPU, CPU, FS);

  return *this;
}

C28xSubtarget::C28xSubtarget(const Triple &TT, const StringRef &CPU,
                             const StringRef &FS, const TargetMachine &TM)
    : C28xGenSubtargetInfo(TT, CPU, CPU, FS), TLInfo(TM, *this),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)),
      FrameLowering(*this){}
