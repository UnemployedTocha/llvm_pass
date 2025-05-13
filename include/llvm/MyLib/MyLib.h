#pragma once

#include <vector>
#include <optional>

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/PatternMatch.h"


namespace llvm {
    class BinaryOperator;
    class Function;
}

using namespace llvm;
using namespace PatternMatch;

namespace mypass {

template <bool FP, typename Mul4Rhs>
static bool matchesSumToSimplify(BinaryOperator &I, Mul4Rhs M4Rhs, Value *&A,
    Value *&B) {
      constexpr unsigned MulOp = FP ? Instruction::FMul : Instruction::Mul;
      constexpr unsigned AddOp = FP ? Instruction::FAdd : Instruction::Add;
      constexpr unsigned SubOp = FP ? Instruction::FSub : Instruction::Sub;
      constexpr unsigned Mul4Op = FP ? Instruction::FMul : Instruction::Shl;
      // constexpr unsigned Mul4Op2 = FP ? Instruction::FMul : Instruction::Shl;

      // (a - b)^2 + 4 * a * c
      //
      // Коммутативно матчит: 
      // (a - b) * (a - b)
      // +
      // ((a * b) * 4) или ((a * 4.0) * b) (для FP чисел или аналог с битовым сдвигом на 2)
      if (match(
        &I,
        m_c_BinOp(
            AddOp,

            m_BinOp(MulOp,
                        m_BinOp(SubOp, m_Value(A), m_Value(B)),
                        m_BinOp(SubOp, m_Deferred(A), m_Deferred(B))),
              
            m_CombineOr(

                m_BinOp(Mul4Op,
                        m_BinOp(MulOp, m_Deferred(A), m_Deferred(B)),
                        M4Rhs),
                
                m_c_BinOp(MulOp,
                        m_BinOp(Mul4Op, m_Deferred(A), M4Rhs),
                        m_Deferred(B)))))) return true;


      return false;
}

Value* replaceSumToSimplify(BinaryOperator* I, Function& F);


std::vector<BinaryOperator*> findSumsToSimplify(Function &F);

} // namespace mypass