#include "mantella_bits/optimisationAlgorithm/randomSearch.hpp"

// C++ standard library
#include <functional>
#include <string>
#include <utility>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  RandomSearch::RandomSearch()
      : OptimisationAlgorithm() {
    setNextParametersFunctions({{[this](
                                     const arma::uword numberOfDimensions_,
                                     const arma::mat& parameters_,
                                     const arma::rowvec& objectiveValues_,
                                     const arma::rowvec& differences_) {
                                   return arma::randu<arma::vec>(numberOfDimensions_);
                                 },
        "Random search"}});
  }

  void RandomSearch::optimise(
      OptimisationProblem& optimisationProblem) {
    // Objects like `optimisationProblem` perform all validations by themselves.
    optimise(optimisationProblem, arma::randu<arma::vec>(optimisationProblem.numberOfDimensions_));
  }
}
