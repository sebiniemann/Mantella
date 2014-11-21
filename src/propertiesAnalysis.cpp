#include <hop_bits/propertiesAnalysis.hpp>

// C++ Standard Library
#include <cstdlib>

namespace hop {
  PropertiesAnalysis::PropertiesAnalysis()
    : plausibility_(0) {

  }

  void PropertiesAnalysis::analyse(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) {
    plausibility_ = 0;

    analyseImplementation(optimisationProblem);
  }

  double PropertiesAnalysis::getPlausibility() const {
    return plausibility_;
  }
}
