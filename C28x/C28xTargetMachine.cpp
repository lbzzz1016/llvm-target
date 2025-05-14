//===-- C28xTargetMachine.cpp - Define TargetMachine for C28x -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about C28x target spec.
//
//===----------------------------------------------------------------------===//

// #include "C28xTargetMachine.h"
#include "C28x.h"

#include "C28xTargetMachine.h"
#include "TargetInfo/C28xTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetOptions.h"

#include <llvm/IR/LegacyPassManager.h>

using namespace llvm;

#define DEBUG_TYPE "c28x"


static Reloc::Model getEffectiveRelocModel(const Triple &TT,
                                           std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

static std::string computeDataLayout(const Triple &TT,
                                     const TargetOptions &Options) {
  return "e-m:e-p:16:16-i32:16-i64:16-f32:16-f64:16-a:8-n8:16-S16";
}

C28xTargetMachine::C28xTargetMachine(const Target &T, const Triple &TT,
                                       StringRef CPU, StringRef FS,
                                       const TargetOptions &Options,
                                       std::optional<Reloc::Model> RM,
                                       std::optional<CodeModel::Model> CM,
                                       CodeGenOptLevel OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, Options), TT, CPU, FS, Options,
                        getEffectiveRelocModel(TT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();

//   // RISC-V supports the MachineOutliner.
//   setMachineOutliner(true);
//   setSupportsDefaultOutlining(true);
}

C28xTargetMachine::~C28xTargetMachine() = default;

// C28x code generation pass config
namespace {

class C28xPassConfig : public TargetPassConfig {
  public:
  C28xPassConfig(C28xTargetMachine &TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  C28xTargetMachine &getTargetMachine() const {
    return getTM<C28xTargetMachine>();
  }

  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *C28xTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new C28xPassConfig(*this, PM);
}

bool C28xPassConfig::addInstSelector() {
  addPass(createC28xDAGToDAGISel(getTargetMachine(), getOptLevel()));
  return false;
}

extern "C" void LLVMInitializeC28xTarget() {
  RegisterTargetMachine<C28xTargetMachine> X(getTheC28xTarget());
  auto &PR = *PassRegistry::getPassRegistry();
  initializeC28xDAGToDAGISelLegacyPass(PR);
}


