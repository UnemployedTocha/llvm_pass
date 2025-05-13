#pragma once

#include "llvm/Analysis/MyPass/SumToSimplify.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"


namespace llvm {

class Function;

} // namespace llvm

namespace mypass {

struct SumToSimplifyTransform : llvm::PassInfoMixin<SumToSimplifyTransform> {
    llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &manager);
  bool run(const mypass::SumToSimplify &sumsToSimplify,
           llvm::Function &F);
};

} // namespace mypass
