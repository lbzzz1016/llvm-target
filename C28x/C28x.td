//===-- MSP430.td - Describe the MSP430 Target Machine -----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// This is the top level entry point for the MSP430 target.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Subtarget Features.
//===----------------------------------------------------------------------===//
def FeatureNoFloat : SubtargetFeature<"nofloat", "FloatModel", "NoFloat",
                                      "no float point supported">;

def FeatureFloat32 : SubtargetFeature<"fpu32", "FloatModel", "Float32",
                                      "32-bit FPU">;

def FeatureLibFloat : SubtargetFeature<"softlib", "FloatModel", "SoftLib",
                                       "software float point supported">;

//===----------------------------------------------------------------------===//
// Subtarget processors, processors have individual pipeline and features
//===----------------------------------------------------------------------===//
class Proc<string Name, list<SubtargetFeature> Features>
  : Processor<Name, NoItineraries, Features>;

def : Proc<"c28x", [FeatureNoFloat, FeatureFloat32, FeatureLibFloat]>;

include "C28xRegisterInfo.td"
include "C28xCallingConv.td"
include "C28xSchedule.td"
include "C28xInstrInfo.td"

def C28xInstrInfo : InstrInfo;


def C28xAsmParser : AsmParser {
  let ShouldEmitMatchRegisterAltName = 1;
}

def C28xAsmWriter : AsmWriter {
  int PassSubtarget = 1;
}

def C28x : Target {
    // def C28xInstrInfo : InstrInfo as before.
  let InstructionSet = C28xInstrInfo;
  let AssemblyParsers = [C28xAsmParser];
}
