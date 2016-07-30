#include "mantella_bits/optimisationAlgorithm/randomSearch.hpp"

// C++ standard library
#include <functional>
#include <string>
#include <utility>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  RandomSearch::RandomSearch()
      : OptimisationAlgorithm() {
    setNextParametersFunctions(
        {{[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_) {
            return uniformRandomNumbers(
                numberOfDimensions_);
          },
          "Random search"}});
  }

  void RandomSearch::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, uniformRandomNumbers(optimisationProblem.numberOfDimensions_));
  }
}
