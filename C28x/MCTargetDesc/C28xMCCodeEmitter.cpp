//===-- C28xMCCodeEmitter.cpp - Convert C28x code to machine code -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the C28xMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/C28xMCTargetDesc.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace {
class C28xMCCodeEmitter : public MCCodeEmitter {
  C28xMCCodeEmitter(const C28xMCCodeEmitter &) = delete;
  void operator=(const C28xMCCodeEmitter &) = delete;
  MCContext &Ctx;
  MCInstrInfo const &MCII;
  unsigned SPPostInc = 0xbd;
  unsigned SPPreDec = 0xbe;

public:
  C28xMCCodeEmitter(MCContext &ctx, MCInstrInfo const &MCII)
      : Ctx(ctx), MCII(MCII) {}

  ~C28xMCCodeEmitter() override {}

  void encodeInstruction(const MCInst &Inst, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  /// TableGen'erated function for getting the binary encoding for an
  /// instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// Return binary encoding of operand. If the machine operand requires
  /// relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getLoc32OpValue(const MCInst &MI, unsigned Op,
                           SmallVectorImpl<MCFixup> &Fixups,
                           const MCSubtargetInfo &STI) const;
};
} // end anonymous namespace

MCCodeEmitter *llvm::createC28xMCCodeEmitter(const MCInstrInfo &MCII,
                                             MCContext &Ctx) {
  return new C28xMCCodeEmitter(Ctx, MCII);
}

void C28xMCCodeEmitter::encodeInstruction(const MCInst &MI,
                                          SmallVectorImpl<char> &CB,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  // Get byte count of instruction.
  unsigned Size = Desc.getSize();

  // CB.push_back(EncodingHigh & 0xFF);
  // CB.push_back((EncodingLow) & 0xFF);
  if (Size == 2) {
    uint16_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
    support::endian::write<uint16_t>(CB, Bits, llvm::endianness::little);
  } else if (Size == 4) {
    uint32_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
    support::endian::write<uint32_t>(CB, Bits, llvm::endianness::little);
  } else {
    llvm::errs() << "wrong size of instruction "
                 << "\n";
  }

  ++MCNumEmitted; // Keep track of the # of mi's emitted.
}

unsigned
C28xMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                     SmallVectorImpl<MCFixup> &Fixups,
                                     const MCSubtargetInfo &STI) const {

  if (MO.isReg()) {
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());
  }

  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());

  llvm_unreachable("Unhandled expression!");
  return 0;
}

unsigned C28xMCCodeEmitter::getLoc32OpValue(const MCInst &MI, unsigned Op,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(Op);
  const MCInst *Val = MO.getInst();
  unsigned Opc = getBinaryCodeForInstr(*Val, Fixups, STI);
  MCOperand Operand = Val->getOperand(0);
  switch (Val->getOpcode()) {
    default: break;
  case C28x::AMPostInc:
    if (Operand.getReg() == C28x::SP) {
      Opc |= 0b111101;
    }else {
      Opc |= (Operand.getReg() - C28x::XAR0);
    }
    break;
  case C28x::AMPreDec:
    if (Operand.getReg() == C28x::SP) {
      Opc |= 0b111110;
    }else {
      Opc |= (0b1000 | (Operand.getReg() - C28x::XAR0));
    }
    break;
  case C28x::AMRegReg:
    Opc |= ((Operand.getReg() - C28x::XAR0) | ((Val->getOperand(1).getReg() - C28x::AR0) << 3));
    // Opc |= ((Val->getOperand(1).getReg() - C28x::AR0) << 3);
    break;
  case C28x::AMRegImm:
    if (Operand.getReg() == C28x::SP) {
      Opc |= Val->getOperand(1).getImm();
    }else {
      Opc |= (0x80 | (Operand.getReg() - C28x::XAR0)) | (Val->getOperand(1).getImm() << 3);
    }
    break;

  }
  return Opc;
}

#include "C28xGenMCCodeEmitter.inc"