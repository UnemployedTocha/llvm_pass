#pragma once

#include <vector>
#include <optional>

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"


namespace llvm {
    class BinaryOperator;
    class Function;
    class raw_ostream;
}

namespace mypass {

class SumToSimplify {
public:
    SumToSimplify(llvm::Function& F, 
        std::vector<llvm::BinaryOperator*>&& BinaryOps);
    llvm::Function* function() const;
    const std::vector<llvm::BinaryOperator*>& subsToSimplify() const;
    void print(llvm::raw_ostream& os) const;
private:
    std::vector<llvm::BinaryOperator*> BinaryOps;
    llvm::Function* F;

};

struct SumToSimplifyAnalysis : llvm::AnalysisInfoMixin<SumToSimplifyAnalysis> {
    using Result = SumToSimplify;
    Result run(llvm::Function& func, llvm::FunctionAnalysisManager& manager);
private:
    friend struct llvm::AnalysisInfoMixin<SumToSimplifyAnalysis>;
    static llvm::AnalysisKey Key;
};

class SumAnalysisPrinter
  : public llvm::PassInfoMixin<SumAnalysisPrinter> {
  llvm::raw_ostream& os;

public:
  explicit SumAnalysisPrinter(llvm::raw_ostream& os);
  llvm::PreservedAnalyses run(llvm::Function& func,
    llvm::FunctionAnalysisManager& manager);
};


} // namespace mypass

