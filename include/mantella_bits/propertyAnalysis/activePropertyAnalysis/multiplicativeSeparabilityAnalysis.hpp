#pragma once

// C++ standard library
#include <memory>
#include <string>
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/propertyAnalysis/activePropertyAnalysis.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class MultiplicativeSeparabilityAnalysis : public ActivePropertyAnalysis {
   public:
    explicit MultiplicativeSeparabilityAnalysis(
        std::shared_ptr<OptimisationProblem> optimisationProblem);

    std::vector<arma::Col<arma::uword>> getPartition();
    double getDeviation();

    std::string toString() const override;

   protected:
    std::vector<arma::Col<arma::uword>> partition_;
    double deviation_;

    void analyseImplementation() override;
  };
}
