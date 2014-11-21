#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp>

namespace hop {
  class QuadraticOrdinaryLeastSquares : public QuadraticModelAnalysis {
    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) override;
  };
}
