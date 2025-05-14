//===-- C28xTargetMachine.h - Define TargetMachine for C28x -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the C28x specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_C28X_C28XTARGETMACHINE_H
#define LLVM_LIB_TARGET_C28X_C28XTARGETMACHINE_H

#include "C28xSubtarget.h"
#include "MCTargetDesc/C28xMCTargetDesc.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include <optional>
// #include "llvm/ADT/Optional.h"


namespace llvm {
class formatted_raw_ostream;
class C28xRegisterInfo;

class C28xTargetMachine : public LLVMTargetMachine {
  // bool isLittle;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  C28xSubtarget Subtarget;
  // Selected ABI
//   C28xABIInfo ABI;

//   mutable StringMap<std::unique_ptr<C28xSubtarget>> SubtargetMap;
public:
  C28xTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                     bool JIT);
  ~C28xTargetMachine() override;


  const C28xSubtarget *getSubtargetImpl(const Function &F) const override {
    return &Subtarget;
  }

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
//   bool isLittleEndian() const { return isLittle; }
//   const C28xABIInfo &getABI() const { return ABI; }
};


} // End llvm namespace

#endif

