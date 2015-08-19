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
  class AdditiveSeparabilityAnalysis : public ActivePropertyAnalysis {
    public:
      explicit AdditiveSeparabilityAnalysis(
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
