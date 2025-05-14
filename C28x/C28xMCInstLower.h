//===-- C28xMCInstLower.h - Lower MachineInstr to MCInst ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_C28X_C28XMCINSTLOWER_H
#define LLVM_LIB_TARGET_C28X_C28XMCINSTLOWER_H

#include "llvm/Support/Compiler.h"

namespace llvm {
class MCContext;
class AsmPrinter;

class MachineInstr;
class MCInst;

// This class is used to lower an MachineInstr to an MCInst
class LLVM_LIBRARY_VISIBILITY C28xMCInstLower {
  MCContext &Ctx;
  AsmPrinter &Printer;

public:
  C28xMCInstLower(MCContext &Ctx, AsmPrinter &Printer)
  : Ctx(Ctx), Printer(Printer) {}

  void lower(const MachineInstr &MI, MCInst &OutMI) const;
};

} // namespace llvm

#endif //C28XMCINSTLOWER_H
