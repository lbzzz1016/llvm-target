//===-- C28xMCAsmInfo.cpp - C28x Asm properties -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the C28xMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "C28xMCAsmInfo.h"
#include "llvm/TargetParser/Triple.h"
using namespace llvm;

void C28xMCAsmInfo::anchor() {}

C28xMCAsmInfo::C28xMCAsmInfo(const Triple &TT) {
  // CodePointerSize = CalleeSaveStackSlotSize = TT.isArch64Bit() ? 8 : 4;
  CodePointerSize = 4;
  CommentString = ";";
  AlignmentIsInBytes = false;
  SupportsDebugInformation = true;
}