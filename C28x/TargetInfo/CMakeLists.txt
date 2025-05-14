add_llvm_component_library(LLVMC28xInfo
  C28xTargetInfo.cpp

  LINK_COMPONENTS
  Support

  LINK_COMPONENTS
  MC

  ADD_TO_COMPONENT
  C28x
  )