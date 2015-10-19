#pragma once

// C++ standard library
#include <memory>
#include <string>

// Mantella
#include "mantella_bits/propertyAnalysis/activePropertyAnalysis.hpp"
namespace mant {
  class OptimisationProblem;
  class RegressionFunction;
}

namespace mant {
  class PolynomialProportionalityAnalysis : public ActivePropertyAnalysis {
   public:
    const std::shared_ptr<RegressionFunction> regressionFunction_;

    explicit PolynomialProportionalityAnalysis(
        std::shared_ptr<OptimisationProblem> optimisationProblem,
        const std::shared_ptr<RegressionFunction> regressionFunction);

    std::string toString() const override;

   protected:
    void analyseImplementation() override;
  };
}
