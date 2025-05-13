#include <llvm/MyLib/MyLib.h>
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

Value* mypass::replaceSumToSimplify(BinaryOperator* I, Function& F) {
  
    assert(I && "binary instruction is null");
  
    IRBuilder<> builder{I};
  
    Value *A, *B;
    if (mypass::matchesSumToSimplify</*FP*/ false>(*I, m_SpecificInt(2), A, B)) {
      Value *AB = builder.CreateAdd(A, B);
      Value *Mul = builder.CreateMul(AB, AB, "square");
      I->replaceAllUsesWith(Mul);
      I->eraseFromParent();
      return AB;
    }
  
    assert(I->hasAllowReassoc() && I->hasNoSignedZeros() && "Assumption mismatch");
  
    if (mypass::matchesSumToSimplify</*FP*/ true>(*I, m_SpecificFP(4.0), A, B)) {
      Value *AB = builder.CreateFAddFMF(A, B, I);
      Value *FMul = builder.CreateFMulFMF(AB, AB, I);
      I->replaceAllUsesWith(FMul);
      I->eraseFromParent();
      return AB;
    }
  
    return nullptr;
    
  }
  
  
std::vector<BinaryOperator*> mypass::findSumsToSimplify(Function &F) {
    std::vector<BinaryOperator*> instructions;
  
    for (BasicBlock &bb : F) {
      for (Instruction &instr : bb) {
        Value* A;
        Value* B;
        
        if (auto *BinOp = dyn_cast<BinaryOperator>(&instr)) {
          if (mypass::matchesSumToSimplify</*FP*/ false>(*BinOp, m_SpecificInt(2), A, B)) {
            instructions.push_back(BinOp);
          }
          // if (matchesSumToSimplify</*FP*/ true>(*BinOp, m_SpecificFP(4.0), A, B)) {
          //   instructions.push_back(BinOp);
          // }
        }
      }
    }
    return instructions;
    
}