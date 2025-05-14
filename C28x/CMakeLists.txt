add_llvm_component_group(C28x)

set(LLVM_TARGET_DEFINITIONS C28x.td)

# tablegen(LLVM MSP430GenAsmMatcher.inc -gen-asm-matcher)
# tablegen(LLVM MSP430GenAsmWriter.inc -gen-asm-writer)
# tablegen(LLVM MSP430GenCallingConv.inc -gen-callingconv)
# tablegen(LLVM MSP430GenDAGISel.inc -gen-dag-isel)
# tablegen(LLVM MSP430GenDisassemblerTables.inc -gen-disassembler)

tablegen(LLVM C28xGenCallingConv.inc -gen-callingconv)
tablegen(LLVM C28xGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM C28xGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM C28xGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM C28xGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM C28xGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM C28xGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM C28xGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(C28xCommonTableGen)

add_llvm_target(C28xCodeGen
  C28xISelDAGToDAG.cpp
  C28xISelLowering.cpp
  C28xInstrInfo.cpp
  C28xFrameLowering.cpp
  C28xRegisterInfo.cpp
  C28xSubtarget.cpp
  C28xTargetMachine.cpp
  C28xAsmPrinter.cpp
  C28xMCInstLower.cpp

  LINK_COMPONENTS
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  MC
  C28xDesc
  C28xInfo
  SelectionDAG
  Support
  Target

  ADD_TO_COMPONENT
  C28x
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
add_subdirectory(AsmParser)
# add_subdirectory(Disassembler)
