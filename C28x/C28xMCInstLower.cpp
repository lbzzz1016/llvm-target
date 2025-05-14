//===-- MSP430MCInstLower.cpp - Convert MSP430 MachineInstr to an MCInst --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower MSP430 MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "C28xMCInstLower.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

#define GET_REGINFO_ENUM
#include "C28xGenRegisterInfo.inc"

void C28xMCInstLower::lower(const MachineInstr &MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI.getOpcode());

  for (const MachineOperand &MO : MI.operands()) {
    MCOperand MCOp;
    switch (MO.getType()) {
      case MachineOperand::MO_Register:
        if (MO.isImplicit()) continue;
        MCOp.createReg(MO.getReg());
        break;
      case MachineOperand::MO_Immediate:
        MCOp.createImm(MO.getImm());
        break;

      default:
        MI.print(errs());
        llvm_unreachable("Unknown operand type");
    }

    OutMI.addOperand(MCOp);
  }
}