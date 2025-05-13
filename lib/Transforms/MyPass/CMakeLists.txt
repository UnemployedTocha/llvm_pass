add_llvm_component_library(SumToSimplifyTransform
    SumToSimplifyTransform.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/MyPass
  
  LINK_LIBS
  SumToSimplifyAnalysis
    
  DEPENDS
    intrinsics_gen
  )