//===-- C28xISelLowering.h - C28x DAG Lowering Interface ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that C28x uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_C28X_C28XISELLOWERING_H
#define LLVM_LIB_TARGET_C28X_C28XISELLOWERING_H

#include "C28x.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
  // Target specified DAG Nodes
  namespace C28xISD {
    enum NodeType : unsigned {
      // Start the numbering where the builtin ops and target ops leave off.
      FIRST_NUMBER = ISD::BUILTIN_OP_END,
      RET,
      SRA,
      SHL,

      RET_GLUE,

      CALL,

      GLOBAL_ADDRESS,
    };
  }

  class C28xSubtarget;
  class C28xTargetLowering : public TargetLowering {

  public:
    explicit C28xTargetLowering(const TargetMachine &TM,
                                const C28xSubtarget &STI);

    SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

    // Returns the name of the target specific DAG node
    const char *getTargetNodeName(unsigned Opcode) const override;
    SDValue LowerShifts(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

  private:
    // These methods must be existed even without function call
    SDValue
    LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                         const SmallVectorImpl<ISD::InputArg> &Ins,
                         const SDLoc &dl, SelectionDAG &DAG,
                         SmallVectorImpl<SDValue> &InVals) const override;
#if 0
    SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                      SmallVectorImpl<SDValue> &InVals) const override;

    bool CanLowerReturn(CallingConv::ID CallConv,
                        MachineFunction &MF,
                        bool IsVarArg,
                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                        LLVMContext &Context) const override;
#endif

    SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                        const SmallVectorImpl<SDValue> &OutVals,
                        const SDLoc &dl, SelectionDAG &DAG) const override;
  };
}

#endif
