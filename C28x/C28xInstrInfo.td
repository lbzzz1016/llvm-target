//===- C28xInstrInfo.td - Target Description for C28x Target -*- tablegen -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the C28x implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// C28x profiles and nodes
//===----------------------------------------------------------------------===//

def SDT_C28xRet          : SDTypeProfile<0, 1, [SDTCisInt<0>]>;

// Return
def C28xRet : SDNode<"C28xISD::RET", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "C28xInstrFormats.td"

//===----------------------------------------------------------------------===//
// C28x Operand, Complex Patterns and Transformations Definitions.
//===----------------------------------------------------------------------===//
// Instruction operand types

// Signed Operand
def simm16      : Operand<i32> {
  let DecoderMethod= "DecodeSimm16";
}

class SImmAsmOperand<int width>
    : AsmOperandClass {
  let Name = "SImm" # width;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def simm12 : Operand<i32> {
  let ParserMatchClass = SImmAsmOperand<12>;
}

class UImmOperand<int width> : AsmOperandClass {
  let Name = "UImm" # width;
  let RenderMethod = "addImmOperands";
  // let DiagnosticType = !strconcat("Invalid", Name);
  let PredicateMethod = "isImm";
}

def uimm4 : Operand<i8> {
  let ParserMatchClass = UImmOperand<4>;
}

def uimm6 : Operand<i8> {
  let ParserMatchClass = UImmOperand<6>;
}

def uimm16 : Operand<i16> {
  let ParserMatchClass = UImmOperand<16>;
}

def DPMemOperand : AsmOperandClass{
  let Name = "DPMem";
  let PredicateMethod = "isImm";
}

def dpmem : Operand<i8> {
  let ParserMatchClass = DPMemOperand;
}


def Loc16Operand : AsmOperandClass{
  let Name = "Loc16";
  let RenderMethod = "addLoc32Operands";
  let PredicateMethod = "isLoc32";
  let ParserMethod = "parseLoc32";
}

def loc16 : Operand<i32>{
  let ParserMatchClass = Loc16Operand;
  let PrintMethod = "printLoc32";
  let EncoderMethod = "getLoc32OpValue";
}

def Loc32Operand : AsmOperandClass{
  let Name = "Loc32";
  let RenderMethod = "addLoc32Operands";
  let PredicateMethod = "isLoc32";
  let ParserMethod = "parseLoc32";
}

def loc32 : Operand<i32>{
  let ParserMatchClass = Loc32Operand;
  let PrintMethod = "printLoc32";
  let EncoderMethod = "getLoc32OpValue";
}


def AMPostInc    : C28xSubInst<0b10000000, (outs), (ins AMBaseRegs:$rs), "*${rs}++","">;
def AMPreDec     : C28xSubInst<0b10000000, (outs), (ins AMBaseRegs:$rs), "*--${rs}","">;
def AMRegReg     : C28xSubInst<0b10010000, (outs), (ins AMBaseRegs:$rs1, AMIndexRegs:$rs2), "*+${rs1}[${rs2}]","">;
def AMRegImm     : C28xSubInst<0b01000000, (outs), (ins AMBaseRegs:$rs1, uimm6:$rs2), "*-${rs1}[${rs2}]","">;



def ABORTI         : C28xInst16Op16<0b0000000000000001, (outs), (ins), "ABORTI", "", []>;
def ABS_ACC        : C28xInst16Op16<0b1111111101010110, (outs), (ins ACCRegs: $rs), "ABS", "$rs", []>;
def ABSTC_ACC      : C28xInst16Op16<0b0101011001011111, (outs), (ins ACCRegs: $rs), "ABSTC", "$rs", []>;
def ADD_ACC_16bit_SHL_15: C28xInst32Op12<0b111111110001, (outs ACCRegs: $rd), (ins uimm16:$rs1, uimm4:$rs2), "ADD", "$rd, $rs1 << $rs2", []>;
def ADD_ACC_loc16_T: C28xInst32Op24<0b010101100010001100000000, (outs ACCRegs: $rd), (ins loc16:$rs1, TRegs:$rs2), "ADD", "$rd, $rs1 << $rs2", []>;
// def ADD_ACC_loc16_0: C28xInst16Op8<0b10000001, (outs ACCRegs:$rd), (ins loc16:$rs), "ADD", " $rd, $rs << #0">;
def ADDL_ACC_LOC32 : C28xInst16Op8<0b00000111, (outs ACCRegs:$rd), (ins loc32:$rs), "ADDL", "$rd, $rs", []>;


//===----------------------------------------------------------------------===//
// Instruction patterns
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Control flow instructions
//===----------------------------------------------------------------------===//
let isReturn = 1, isTerminator = 1, isBarrier = 1 in {
  def LRET : C28xInst16Op16<0b0111011000010100,
                            (outs), (ins), "LRET", "", []> {
    let Pattern = [(C28xRet)];
  }
}
