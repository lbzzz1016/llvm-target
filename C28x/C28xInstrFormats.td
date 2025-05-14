//===-- C28xInstrFormats.td - C28x Instruction Formats -----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//


class C28xInstBase<dag outs, dag ins, string opcodestr, string argstr, list<dag> pattern> : Instruction {
  field bits<16> SoftFail = 0;
  let Namespace = "C28x";

  dag OutOperandList = outs;
  dag InOperandList  = ins;

  let AsmString = opcodestr # !if(!empty(argstr), "", "\t" # argstr);
  let Pattern = pattern;
}

class C28xInst16<dag outs, dag ins, string opcodestr, string argstr, list<dag> pattern> : C28xInstBase<outs, ins, opcodestr, argstr, pattern> {
  field bits<16> Inst = 0;
  let Size = 2;
}

class C28xInst16Op16<bits<16> opcode, dag outs, dag ins, string opcodestr, string argstr, list<dag> pattern> : C28xInst16<outs, ins, opcodestr, argstr, pattern> {
  let Inst{15-0} = opcode;
}

class C28xInst16Op8<bits<8> opcode, dag outs, dag ins, string opcodestr, string argstr, list<dag> pattern> : C28xInst16<outs, ins, opcodestr, argstr, pattern> {
  let Inst{15-8} = opcode;
  bits<8> rs;
  let Inst{7-0} = rs;
}

class C28xInst32<dag outs, dag ins, string opcodestr, string argstr, list<dag> pattern> : C28xInstBase<outs, ins, opcodestr, argstr, pattern> {
  field bits<32> Inst = 0;
  let Size = 4;
}

class C28xInst32Op8<bits<8> opcode, dag outs, dag ins, string opcodestr, string argstr, list<dag> pattern> : C28xInst32<outs, ins, opcodestr, argstr, pattern> {
  let Inst{31-24} = opcode;
  bits<8> rd;
  bits<16> rs;
  let Inst{23-16} = rd;
  let Inst{15-0} = rs;
}


class C28xInst32Op12<bits<12> opcode, dag outs, dag ins, string opcodestr, string argstr, list<dag> pattern> : C28xInst32<outs, ins, opcodestr, argstr, pattern> {
  let Inst{31-20} = opcode;
  bits<16> rs1;
  bits<4> rs2;
  let Inst{15-0} = rs1;
  let Inst{19-16} = rs2;

}

class C28xInst32Op24<bits<24> opcode, dag outs, dag ins, string opcodestr, string argstr, list<dag> pattern> : C28xInst32<outs, ins, opcodestr, argstr, pattern> {
  let Inst{31-8} = opcode;
  bits<8> rs1;
  let Inst{7-0} = rs1;
}

class C28xSubInst<bits<8> opcode, dag outs, dag ins, string opcodestr, string argstr> : C28xInstBase<outs, ins, opcodestr, argstr, []> {
  field bits<8> Inst = opcode;
  let Size = 1;
  let isCodeGenOnly = true;
  // let isPseudo = true;
}


