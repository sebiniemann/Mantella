#pragma once

// C++ standard library
#include <memory>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/propertyAnalysis/activePropertyAnalysis.hpp>
#include <mantella_bits/optimisationProblem.hpp>

namespace mant {
  class MultiplicativeSeparabilityAnalysis : public ActivePropertyAnalysis {
    public:
      explicit MultiplicativeSeparabilityAnalysis(
          std::shared_ptr<OptimisationProblem> optimisationProblem);

      void setMaximalNumberOfIterations(
            const arma::uword maximalNumberOfIterations);

      std::string toString() const override;
            
    protected:
      arma::uword maximalNumberOfIterations_;

      void analyseImplementation() override;
  };
}
