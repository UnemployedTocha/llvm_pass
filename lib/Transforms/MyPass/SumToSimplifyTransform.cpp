#include "llvm/Transforms/MyPass/SumToSimplifyTransform.h"

#include <cassert>

#include "llvm/MyLib/MyLib.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"


llvm::PreservedAnalyses
mypass::SumToSimplifyTransform::run(llvm::Function &F,
                                       llvm::FunctionAnalysisManager &manager) {
  auto &sumsToSimplify = manager.getResult<SumToSimplifyAnalysis>(F);
  bool modified = run(sumsToSimplify, F);
  return modified ? llvm::PreservedAnalyses::none()
                  : llvm::PreservedAnalyses::all();
}

bool mypass::SumToSimplifyTransform::run(
    const mypass::SumToSimplify &sumsToSimplify,
    llvm::Function &F) {
  bool modified{false};
  // Functions marked explicitly 'optnone' should be ignored since we shouldn't
  // be changing anything in them anyway.
  if (sumsToSimplify.function()->hasFnAttribute(
          llvm::Attribute::OptimizeNone)) {
    return modified;
  }

  for (llvm::BinaryOperator *binOp :
       sumsToSimplify.subsToSimplify()) {
    if (mypass::replaceSumToSimplify(binOp, F)) {
      modified = true;
    }
  }

  return modified;
}