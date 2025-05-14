//===-- C28xMCAsmInfo.h - C28x Asm Info ----------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the C28xMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_C28X_MCTARGETDESC_C28XMCASMINFO_H
#define LLVM_LIB_TARGET_C28X_MCTARGETDESC_C28XMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class C28xMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit C28xMCAsmInfo(const Triple &TargetTriple);
};

} // namespace llvm

#endif