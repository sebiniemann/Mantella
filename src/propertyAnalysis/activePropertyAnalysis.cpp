#include "mantella_bits/propertyAnalysis/activePropertyAnalysis.hpp"

// Mantella
#include "mantella_bits/helper/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  ActivePropertyAnalysis::ActivePropertyAnalysis(
      std::shared_ptr<OptimisationProblem> optimisationProblem)
      : optimisationProblem_(optimisationProblem) {
    setLowerBounds(optimisationProblem_->getLowerBounds());
    setUpperBounds(optimisationProblem_->getUpperBounds());
  }

  void ActivePropertyAnalysis::setLowerBounds(
      const arma::Col<double>& lowerBounds) {
    lowerBounds_ = lowerBounds;
  }

  void ActivePropertyAnalysis::setUpperBounds(
      const arma::Col<double>& upperBounds) {
    upperBounds_ = upperBounds;
  }

  arma::Col<double> ActivePropertyAnalysis::getLowerBounds() const {
    return lowerBounds_;
  }

  arma::Col<double> ActivePropertyAnalysis::getUpperBounds() const {
    return upperBounds_;
  }

  void ActivePropertyAnalysis::setMaximalNumberOfIterations(
      const arma::uword maximalNumberOfIterations) {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  arma::uword ActivePropertyAnalysis::getMaximalNumberOfIterations() {
    return maximalNumberOfIterations_;
  }

  void ActivePropertyAnalysis::analyse() {
    verify(arma::all(optimisationProblem_->getLowerBounds() <= optimisationProblem_->getUpperBounds()), "All upper bounds of the optimisation problem must be greater than or equal to its lower bound.");

    analyseImplementation();
  }
}
