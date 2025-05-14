//===-- C28xMCTargetDesc.cpp - C28x Target Descriptions -------------------===//
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

#include "C28xMCTargetDesc.h"
#include "C28xMCAsmInfo.h"
#include "C28xInstPrinter.h"
#include "TargetInfo/C28xTargetInfo.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include <bitset>

#define GET_SUBTARGETINFO_MC_DESC
#include "C28xGenSubtargetInfo.inc"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "C28xGenInstrInfo.inc"

// #define GET_SUBTARGETINFO_MC_DESC
// #include "C28xGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "C28xGenRegisterInfo.inc"


static MCInstrInfo *createC28xMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitC28xMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createC28xMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitC28xMCRegisterInfo(X, C28x::XAR1);
  return X;
}

static MCAsmInfo *createC28xMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT,
                                       const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new C28xMCAsmInfo(TT);
  return MAI;
}

static MCInstPrinter *createC28xMCInstPrinter(const Triple &T,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new C28xInstPrinter(MAI, MII, MRI);
  return nullptr;
}

static MCSubtargetInfo *
createC28xMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createC28xMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

//@2 {
extern "C" void LLVMInitializeC28xTargetMC() {
    Target &T = getTheC28xTarget();

    TargetRegistry::RegisterMCAsmInfo(T, createC28xMCAsmInfo);
    TargetRegistry::RegisterMCInstrInfo(T, createC28xMCInstrInfo);
    TargetRegistry::RegisterMCRegInfo(T, createC28xMCRegisterInfo);
    TargetRegistry::RegisterMCAsmBackend(T, createC28xAsmBackend);
    TargetRegistry::RegisterMCCodeEmitter(T, createC28xMCCodeEmitter);
    TargetRegistry::RegisterMCInstPrinter(T, createC28xMCInstPrinter);
    TargetRegistry::RegisterMCSubtargetInfo(T, createC28xMCSubtargetInfo);
    // todo 

  
}
//@2 }
