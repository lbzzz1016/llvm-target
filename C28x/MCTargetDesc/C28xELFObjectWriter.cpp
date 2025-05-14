//===-- C28xELFObjectWriter.cpp - C28x ELF Writer -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/C28xMCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class C28xELFObjectWriter : public MCELFObjectTargetWriter {
public:
  C28xELFObjectWriter(uint8_t OSABI);

  ~C28xELFObjectWriter() override;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};
}

C28xELFObjectWriter::C28xELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(false, OSABI, ELF::EM_C28X,
                              /*HasRelocationAddend*/ true) {}

C28xELFObjectWriter::~C28xELFObjectWriter() {}

unsigned C28xELFObjectWriter::getRelocType(MCContext &Ctx,
                                            const MCValue &Target,
                                            const MCFixup &Fixup,
                                            bool IsPCRel) const {
  report_fatal_error("invalid fixup kind!");
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createC28xELFObjectWriter(uint8_t OSABI) {
  return 
      std::make_unique<C28xELFObjectWriter>(OSABI);
}