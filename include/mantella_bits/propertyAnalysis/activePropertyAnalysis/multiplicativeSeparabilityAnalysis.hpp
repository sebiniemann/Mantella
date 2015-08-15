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

      arma::uword getMaximalNumberOfIterations();
      
      std::vector<arma::Col<arma::uword>> getPartition();
      double getDeviation();

      std::string toString() const override;
            
    protected:
      arma::uword maximalNumberOfIterations_;
      
      std::vector<arma::Col<arma::uword>> partition_;
      double deviation_;

      void analyseImplementation() override;
  };
}
