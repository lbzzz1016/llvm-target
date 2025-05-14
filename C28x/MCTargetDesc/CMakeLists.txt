# MCTargetDesc/CMakeLists.txt
add_llvm_component_library(LLVMC28xDesc
  C28xAsmBackend.cpp
  C28xELFObjectWriter.cpp
  C28xMCAsmInfo.cpp
  C28xMCTargetDesc.cpp
  C28xMCCodeEmitter.cpp
  C28xInstPrinter.cpp


  LINK_COMPONENTS
  MC
  C28xInfo
  Support

  ADD_TO_COMPONENT
  C28x
  )