add_llvm_component_library(MyLib
  MyLib.cpp
  
  ADDITIONAL_HEADER_DIRS
    ${LLVM_MAIN_INCLUDE_DIRS}/llvm/MyLib
    
  DEPENDS
    intrinsics_gen
  )