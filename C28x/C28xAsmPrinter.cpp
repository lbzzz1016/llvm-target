//===-- C28xAsmPrinter.cpp - C28x LLVM assembly writer ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the C28x assembly language.
//
//===----------------------------------------------------------------------===//

#include "C28x.h"
#include "C28xMCInstLower.h"
#include "TargetInfo/C28xTargetInfo.h"
#include "MCTargetDesc/C28xInstPrinter.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;
#define DEBUG_TYPE "c28x-asm-printer"
#define PASS_NAME "C28x Assembly Printer"

namespace {

class C28xAsmPrinter : public AsmPrinter {
public:
  C28xAsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
  : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return PASS_NAME; }

  bool runOnMachineFunction(MachineFunction &MF) override;

  void emitInstruction(const MachineInstr *MI) override;
};

} // end of anonymous namespace

void C28xAsmPrinter::emitInstruction(const MachineInstr *MI) {
  C28xMCInstLower MCInstLowering(OutContext, *this);

  // Lower to MC then print
  MCInst TmpInst;
  MCInstLowering.lower(*MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

// Entrance of function pass
bool C28xAsmPrinter::runOnMachineFunction(MachineFunction &MF) {
  // TODO Interrupt function

  SetupMachineFunction(MF);
  emitFunctionBody();
  return false;
}

// Register pass
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeC28xAsmPrinter() {
  RegisterAsmPrinter<C28xAsmPrinter> X(getTheC28xTarget());
}