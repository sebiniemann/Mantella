#include "mantella_bits/optimisationAlgorithm/randomSearch.hpp"

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  RandomSearch::RandomSearch()
      : OptimisationAlgorithm() {
    setNextParametersFunction(
        [this](
            const arma::uword numberOfDimensions,
            const arma::Mat<double>& parameters,
            const arma::Col<double>& objectiveValues,
            const arma::Col<double>& differences) {
          return arma::randu<arma::Col<double>>(parameters.n_rows);
        },
        "Random search");
  }

  void RandomSearch::optimise(
      OptimisationProblem& optimisationProblem) {
    OptimisationAlgorithm::optimise(optimisationProblem, arma::Mat<double>(optimisationProblem.numberOfDimensions_, 0));
  }
}
