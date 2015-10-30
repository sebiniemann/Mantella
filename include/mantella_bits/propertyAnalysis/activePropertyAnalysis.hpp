#pragma once

// C++ standard library
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/propertyAnalysis.hpp>
#include <mantella_bits/optimisationProblem.hpp>

namespace mant {
  class ActivePropertyAnalysis : public PropertyAnalysis {
   public:
    explicit ActivePropertyAnalysis(
        std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setLowerBounds(
        const arma::Col<double>& lowerBounds);
    void setUpperBounds(
        const arma::Col<double>& upperBounds);

    arma::Col<double> getLowerBounds() const;
    arma::Col<double> getUpperBounds() const;

    void setMaximalNumberOfIterations(
        const arma::uword maximalNumberOfIterations);

    arma::uword getMaximalNumberOfIterations();

    void analyse();

   protected:
    std::shared_ptr<OptimisationProblem> optimisationProblem_;

    arma::Col<double> lowerBounds_;
    arma::Col<double> upperBounds_;

    arma::uword maximalNumberOfIterations_;

    virtual void analyseImplementation() = 0;
  };
}
