//===-- C28xRegisterInfo.td - C28x Register defs -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the C28x register file
//===----------------------------------------------------------------------===//

class C28xReg<bits<16> Enc, string n> : Register<n> {
  // For tablegen(... -gen-emitter)  in CMakeLists.txt
  let HWEncoding = Enc;
  let Namespace = "C28x";
}

class C28xRegWithSubregs<bits<16> Enc, string n, list<Register> subregs>
  : RegisterWithSubRegs<n, subregs> {
  let HWEncoding = Enc;
  let Namespace = "C28x";
}


//===----------------------------------------------------------------------===//
//@Registers
//===----------------------------------------------------------------------===//
// The register string, such as "9" or "gp" will show on "llvm-objdump -d"
//@ All registers definition, DwarfRegNum defined by spec <C28x EABI>

// 16-bit registers
def AL  : C28xReg<0x0, "AL">,   DwarfRegNum<[0]>;
def AH  : C28xReg<0x1, "AH">,   DwarfRegNum<[1]>;
def PL  : C28xReg<0x0, "PL">,   DwarfRegNum<[2]>;
def PH  : C28xReg<0x1, "PH">,   DwarfRegNum<[3]>;
def AR0 : C28xReg<0x0, "AR0">,  DwarfRegNum<[4]>;
def AR1 : C28xReg<0x1, "AR1">,  DwarfRegNum<[6]>;
def AR2 : C28xReg<0x2, "AR2">,  DwarfRegNum<[8]>;
def AR3 : C28xReg<0x3, "AR3">,  DwarfRegNum<[10]>;
def AR4 : C28xReg<0x4, "AR4">,  DwarfRegNum<[12]>;
def AR5 : C28xReg<0x5, "AR5">,  DwarfRegNum<[14]>;
def AR6 : C28xReg<0x6, "AR6">,  DwarfRegNum<[16]>;
def AR7 : C28xReg<0x7, "AR7">,  DwarfRegNum<[18]>;
def SP  : C28xReg<0xad,"SP">,   DwarfRegNum<[20]>;
def TL  : C28xReg<0x0, "TL">,   DwarfRegNum<[21]>;
def T   : C28xReg<0x1, "T">,    DwarfRegNum<[22]>;
def ST0 : C28xReg<0x0, "ST0">,  DwarfRegNum<[23]>;
def ST1 : C28xReg<0x1, "ST1">,  DwarfRegNum<[24]>;
def DP  : C28xReg<9,   "DP">,   DwarfRegNum<[29]>;
def IFR : C28xReg<10,  "IFR">,  DwarfRegNum<[36]>;
def IER : C28xReg<11,  "IER">,  DwarfRegNum<[37]>;
// Dwarf number is undefined for DBGIER
def DBGIER : C28xReg<12,  "DBGIER">,DwarfRegNum<[-1]>;

// 22-bit registers
def PC  : C28xReg<14,   "PC">,  DwarfRegNum<[25]>;
def RPC : C28xReg<15,   "RPC">, DwarfRegNum<[26]>;

// 32-bit register containing sub-register
// SubRegIndex(size, offset = 0)
def SubReg_LSB16 : SubRegIndex<16, 0> {let Namespace = "C28x"; }
def SubReg_MSB16 : SubRegIndex<16, 16> {let Namespace = "C28x"; }

let SubRegIndices = [SubReg_LSB16, SubReg_MSB16] in {
def ACC : C28xRegWithSubregs<0xa9,  "ACC",  [AL, AH]>, DwarfRegNum<[-1]>;
def P   : C28xRegWithSubregs<0xab,  "P",    [PL, PH]>, DwarfRegNum<[-1]>;
def XT  : C28xRegWithSubregs<0xac,  "XT",   [TL, T] >, DwarfRegNum<[-1]>;
}

let SubRegIndices = [SubReg_LSB16] in {
def XAR0 : C28xRegWithSubregs<0xa0, "XAR0", [AR0]>, DwarfRegNum<[5]>;
def XAR1 : C28xRegWithSubregs<0xa1, "XAR1", [AR1]>, DwarfRegNum<[7]>;
def XAR2 : C28xRegWithSubregs<0xa2, "XAR2", [AR2]>, DwarfRegNum<[9, 28]> {
  let AltNames = ["FP"];
}
def XAR3 : C28xRegWithSubregs<0xa3, "XAR3", [AR3]>, DwarfRegNum<[11]>;
def XAR4 : C28xRegWithSubregs<0xa4, "XAR4", [AR4]>, DwarfRegNum<[13]>;
def XAR5 : C28xRegWithSubregs<0xa5, "XAR5", [AR5]>, DwarfRegNum<[15]>;
def XAR6 : C28xRegWithSubregs<0xa6, "XAR6", [AR6]>, DwarfRegNum<[17]>;
def XAR7 : C28xRegWithSubregs<0xa7, "XAR7", [AR7]>, DwarfRegNum<[19]>;
}

// 64-bit register pairs according to ABI
def SubReg_LSB32 : SubRegIndex<32, 0> { let Namespace = "C28x"; }
def SubReg_MSB32 : SubRegIndex<32, 32> { let Namespace = "C28x"; }
def R64 : RegisterTuples< [SubReg_MSB32, SubReg_LSB32],
    [(add ACC, XAR1, XAR3, XAR5, XAR7), (add P, XAR0, XAR2, XAR4, XAR6)]>;


def CPURegs : RegisterClass<"C28x", [i32], 32, (add
  ACC, (sequence "XAR%u", 0, 7),
  // Return Values and Arguments
  DP, IFR, IER, DBGIER, 
  // Not preserved across procedure calls
  P, PC, RPC,
  // Callee save
  SP, ST0, ST1, XT
  )>;

//@Status Registers class
def StatusRegs     : RegisterClass<"C28x", [i32], 32, (add ST0, ST1)>;

//@Co-processor 0 Registers class
def AuxRegs : RegisterClass<"C28x", [i32], 32, (add XAR0, XAR1, XAR2, XAR3, XAR4, XAR5, XAR6, XAR7)>;
def AuxRegs16 : RegisterClass<"C28x", [i16], 16, (add (sequence "AR%u", 0, 7))>;

def GPR : RegisterClass<"C28x", [i32], 32, (add CPURegs)>;

def ACCRegs : RegisterClass<"C28x", [i32], 32, (add ACC)>;
def AHRegs : RegisterClass<"C28x", [i16], 16, (add AH)>;
def PRegs : RegisterClass<"C28x", [i32], 32, (add P)>;
def SPRegs : RegisterClass<"C28x", [i32], 32, (add SP)>;
def TRegs : RegisterClass<"C28x", [i32], 32, (add T)>;


def AMBaseRegs: RegisterClass<"C28x", [i32], 32, (add SP,AuxRegs)>;
def AMIndexRegs: RegisterClass<"C28x", [i16], 16, (add AR0, AR1)>;



