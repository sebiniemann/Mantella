#include <mantella_bits/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/randomSearch.hpp>

namespace mant {
  void RandomSearch::optimiseImplementation() {
    while (!isFinished() && !isTerminated()) {
      ++numberOfIterations_;
    
      const arma::Col<double>& candidateParameter = getRandomParameter();
      updateBestParameter(candidateParameter, getSoftConstraintsValue(candidateParameter), getObjectiveValue(candidateParameter));
    }
  }

  std::string RandomSearch::toString() const {
    return "random_search";
  }
}
