add_llvm_component_library(SumToSimplifyAnalysis
  SumToSimplify.cpp
  
  ADDITIONAL_HEADER_DIRS
    ${LLVM_MAIN_INCLUDE_DIR}/llvm/Analysis
    ${LLVM_MAIN_INCLUDE_DIR}/llvm/Analysis/MyPass

  LINK_LIBS
  MyLib

  DEPENDS
    intrinsics_gen
  )

set_target_properties(SumToSimplifyAnalysis
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
  )