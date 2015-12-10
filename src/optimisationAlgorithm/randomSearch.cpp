#include "mantella_bits/optimisationAlgorithm/randomSearch.hpp"

// Mantella
#include "mantella_bits/probability.hpp"

namespace mant {
  RandomSearch::RandomSearch()
      : OptimisationAlgorithm() {
    setNextParametersFunction([this] (
        const arma::Mat<double>& parameters,
        const arma::Col<double>& differences) {
      return arma::randu<arma::Col<double>>(parameters.n_rows);
    });
  }
  
  void RandomSearch::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::Mat<double>& initialParameters) {
    verify(initialParameters.is_empty(), "optimise: The random search algorithm does not accept initial parameters.");
    
    OptimisationAlgorithm::optimise(optimisationProblem, initialParameters);
  }
  
  void RandomSearch::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, arma::Mat<double>(optimisationProblem.numberOfDimensions_, 0));
  }
}
