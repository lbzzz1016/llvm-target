//===-- C28xInstPrinter.cpp - Convert C28x MCInst to asm syntax ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an C28x MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "C28xInstPrinter.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCRegister.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#include "C28xGenAsmWriter.inc"

void C28xInstPrinter::printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                 const MCSubtargetInfo &STI, raw_ostream &O) {
  printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

void C28xInstPrinter::printRegName(raw_ostream &O, MCRegister Reg) const {
  markup(O, Markup::Register) << getRegisterName(Reg);
}

void C28xInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                     raw_ostream &O,
                                    const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &MO = MI->getOperand(OpNo);
//   print();
  if (MO.isReg()) {
    printRegName(O, MO.getReg());
    return;
  }

  if (MO.isImm()) {
    markup(O, Markup::Immediate) << '#' << formatImm(MO.getImm());
    return;
  }



  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI);
}

void C28xInstPrinter::printLoc32(const MCInst *MI, unsigned OpNo, raw_ostream &O)  {
  const MCOperand &MO = MI->getOperand(OpNo);
  if (MO.isInst()) {
    const MCInst *Val = MO.getInst();
    switch (Val->getOpcode()) {
    case C28x::AMPostInc:
      markup(O, Markup::Register) << "*" << getRegisterName(Val->getOperand(0).getReg()) << "++";
      break;
    case C28x::AMPreDec:
      markup(O, Markup::Register) << "*--" << getRegisterName(Val->getOperand(0).getReg());
      break;
    case C28x::AMRegReg:
      markup(O, Markup::Register) << "*+" << getRegisterName(Val->getOperand(0).getReg()) << "[" << getRegisterName(Val->getOperand(1).getReg()) << "]";
      break;
    case C28x::AMRegImm:
      unsigned Reg = Val->getOperand(0).getReg();
      StringRef Sym = "+";
      if (Reg == C28x::SP) {
        Sym = "-";
      }
      markup(O, Markup::Register) << "*" << Sym << getRegisterName(Reg) << "[" << Val->getOperand(1).getImm() << "]";
      break;

    }
    delete Val;
  }

  if (MO.isReg()) {
    printRegName(O, MO.getReg());
    return;
  }

  if (MO.isImm()) {
    markup(O, Markup::Immediate) << '#' << formatImm(MO.getImm());
    return;
  }
}