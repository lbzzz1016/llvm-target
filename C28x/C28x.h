//===-- C28x.h - Top-level interface for C28x representation ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM C28x back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_C28x_C28x_H
#define LLVM_LIB_TARGET_C28x_C28x_H

// #include "C28xConfig.h"
#include "MCTargetDesc/C28xMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class C28xTargetMachine;
  class FunctionPass;
  class PassRegistry;

  FunctionPass *createC28xDAGToDAGISel(C28xTargetMachine &TM,
                                       CodeGenOptLevel Level);
  void initializeC28xDAGToDAGISelLegacyPass(PassRegistry &);

} // end namespace llvm;

#endif