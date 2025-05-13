#include <llvm/Analysis/MyPass/SumToSimplify.h>
#include "llvm/MyLib/MyLib.h"

#include "llvm/IR/ModuleSlotTracker.h"

using namespace llvm;

namespace mypass {
    static constexpr char PassName[] = "my-sum-pass-analysis";
    static constexpr char PassDescription[] = "Find expr like (a - b)^2 + 4 * a * b";

} // namespace mypass                                 

#define DEBUG_TYPE mypass::PassName

mypass::SumToSimplify::SumToSimplify(llvm::Function& F,
                                    std::vector<llvm::BinaryOperator*>&& BinaryOps) {
    this->F = &F;
    this->BinaryOps = std::move(BinaryOps);
}

llvm::Function* mypass::SumToSimplify::function() const {
    return F;
}

const std::vector<llvm::BinaryOperator*>& mypass::SumToSimplify::subsToSimplify() const {
    return BinaryOps;
}

void mypass::SumToSimplify::print(llvm::raw_ostream& os) const {
  if (!BinaryOps.empty())
  { 
    llvm::ModuleSlotTracker slotTracker{F->getParent()};
    os << "Expression like ((a - b)^2 + 4 * a * b) found in \"" << F->getName() << "\":\n";
    for (llvm::BinaryOperator* BinaryOp : BinaryOps) {
      BinaryOp->print(os, slotTracker);
      os << '\n';
    }
  }    
}


mypass::SumToSimplifyAnalysis::Result mypass::SumToSimplifyAnalysis::run(
  llvm::Function& F, llvm::FunctionAnalysisManager& manager)
{

  std::vector<BinaryOperator*> instructions = mypass::findSumsToSimplify(F);
  SumToSimplify res{F, std::move(instructions)};
  return res;

}

llvm::AnalysisKey mypass::SumToSimplifyAnalysis::Key{};

mypass::SumAnalysisPrinter::SumAnalysisPrinter(
  llvm::raw_ostream& os) : os(os){ }

llvm::PreservedAnalyses mypass::SumAnalysisPrinter::run(
  llvm::Function& F, llvm::FunctionAnalysisManager& manager) {

    auto sumsToSimplify = manager.getResult<SumToSimplifyAnalysis>(F);
    sumsToSimplify.print(os);    
    return llvm::PreservedAnalyses::all();

}