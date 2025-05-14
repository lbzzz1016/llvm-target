//===-- C28xTargetInfo.cpp - C28x Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "C28x.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"
#include "TargetInfo/C28xTargetInfo.h"

using namespace llvm;

Target &llvm::getTheC28xTarget() { // The global target instance.
   static Target TheC28xTarget;
   return TheC28xTarget;
}


extern "C" void LLVMInitializeC28xTargetInfo() {
  RegisterTarget<Triple::c28x,false> X(getTheC28xTarget(), "c28x", "C28x 32-bit little endian", "C28x");
  
}