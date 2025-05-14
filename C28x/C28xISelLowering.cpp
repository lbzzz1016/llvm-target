//===-- C28xISelLowering.cpp - C28x DAG Lowering Implementation  ------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the C28xTargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "C28xISelLowering.h"
#include "C28x.h"
#include "C28xSubtarget.h"
#include "C28xTargetMachine.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

using namespace llvm;
#define DEBUG_TYPE "c28x-isel-lowering"

C28xTargetLowering::C28xTargetLowering(const TargetMachine &TM,
                                       const C28xSubtarget &STI)
    : TargetLowering(TM) {
  // Set up value type of register classes
  addRegisterClass(MVT::i32, &C28x::AuxRegsRegClass);
  addRegisterClass(MVT::i16, &C28x::AuxRegs16RegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(STI.getRegisterInfo());

  setStackPointerRegisterToSaveRestore(C28x::SP);

  setMinFunctionAlignment(Align(2));
  setPrefFunctionAlignment(Align(2));
}

SDValue C28xTargetLowering::LowerOperation(SDValue Op,
                                             SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::SHL: // FALLTHROUGH
  case ISD::SRL:
  case ISD::SRA:              return LowerShifts(Op, DAG);
  case ISD::GlobalAddress:    return LowerGlobalAddress(Op, DAG);
  // case ISD::BlockAddress:     return LowerBlockAddress(Op, DAG);
  // case ISD::ExternalSymbol:   return LowerExternalSymbol(Op, DAG);
  // case ISD::SETCC:            return LowerSETCC(Op, DAG);
  // case ISD::BR_CC:            return LowerBR_CC(Op, DAG);
  // case ISD::SELECT_CC:        return LowerSELECT_CC(Op, DAG);
  // case ISD::SIGN_EXTEND:      return LowerSIGN_EXTEND(Op, DAG);
  // case ISD::RETURNADDR:       return LowerRETURNADDR(Op, DAG);
  // case ISD::FRAMEADDR:        return LowerFRAMEADDR(Op, DAG);
  // case ISD::VASTART:          return LowerVASTART(Op, DAG);
  // case ISD::JumpTable:        return LowerJumpTable(Op, DAG);
  default:
    llvm_unreachable("unimplemented operand");
  }
}

const char *C28xTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (static_cast<C28xISD::NodeType>(Opcode)) {
  case C28xISD::FIRST_NUMBER:     break;
  case C28xISD::SRA:              break;
  case C28xISD::SHL:              break;
  case C28xISD::GLOBAL_ADDRESS:   break;
  case C28xISD::RET:             return "C28x::RET";
  }

  return nullptr;
}


SDValue C28xTargetLowering::LowerShifts(SDValue Op, SelectionDAG &DAG) const {

  SDLoc dl(Op);
  SDValue Operand = Op.getOperand(0);
  SDValue ShiftAmount = Op.getOperand(1);

  if (Op.getOpcode() == ISD::SRA) {
    return DAG.getNode(C28xISD::SRA, dl, MVT::i16, Operand, ShiftAmount);
  } else if (Op.getOpcode() == ISD::SHL) {
    return DAG.getNode(C28xISD::SHL, dl, MVT::i16, Operand, ShiftAmount);
  }
  
  return SDValue();
}

SDValue C28xTargetLowering::LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const {
  GlobalAddressSDNode *GA = cast<GlobalAddressSDNode>(Op);
  const GlobalValue *GV = GA->getGlobal();
  int64_t Offset = GA->getOffset();
  EVT PtrVT = Op.getValueType();


  SDValue GAVal = DAG.getGlobalAddress(GV, SDLoc(Op), PtrVT, Offset);
  
  return DAG.getNode(C28xISD::GLOBAL_ADDRESS, SDLoc(Op), PtrVT, GAVal,
                     DAG.getTargetConstant(0, SDLoc(Op), MVT::i32));

}


SDValue C28xTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  return Chain;
}

SDValue
C28xTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                bool isVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &dl, SelectionDAG &DAG) const {
  return DAG.getNode(C28xISD::RET, dl, MVT::Other, Chain);
}
