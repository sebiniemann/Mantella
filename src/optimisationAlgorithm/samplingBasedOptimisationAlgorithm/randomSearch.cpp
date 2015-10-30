#include "mantella_bits/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/randomSearch.hpp"

// Armadillo
namespace arma {
  template <typename eT>
  class Col;
}

namespace mant {
  void RandomSearch::optimiseImplementation() {
    while (!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      const arma::Col<double>& candidateParameter = getRandomParameter();
      updateBestParameter(candidateParameter, getObjectiveValue(candidateParameter));
    }
  }

  std::string RandomSearch::toString() const {
    return "random_search";
  }
}
