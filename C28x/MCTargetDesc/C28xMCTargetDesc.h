//===-- C28xMCTargetDesc.h - C28x Target Descriptions -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides C28x specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_C28X_MCTARGETDESC_C28XMCTARGETDESC_H
#define LLVM_LIB_TARGET_C28X_MCTARGETDESC_C28XMCTARGETDESC_H

// #include "C28xConfig.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class Target;



MCCodeEmitter *createC28xMCCodeEmitter(const MCInstrInfo &MCII,
                                              MCContext &Ctx);

MCAsmBackend *createC28xAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter>
createC28xELFObjectWriter(uint8_t OSABI);



extern Target TheC28xTarget;

} // End llvm namespace

// Defines symbolic names for C28x registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "C28xGenRegisterInfo.inc"

// Defines symbolic names for the C28x instructions.
#define GET_INSTRINFO_ENUM
#include "C28xGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "C28xGenSubtargetInfo.inc"

#endif
