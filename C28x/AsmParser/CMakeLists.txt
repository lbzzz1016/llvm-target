add_llvm_component_library(LLVMC28xAsmParser
  C28xAsmParser.cpp

  LINK_COMPONENTS
  CodeGen
  MC
  MCParser
  C28xDesc
  C28xInfo
  Support
  TargetParser

  ADD_TO_COMPONENT
  C28x
  )