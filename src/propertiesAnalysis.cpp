#include <hop_bits/propertiesAnalysis.hpp>

// C++ Standard Library
#include <cstdlib>

namespace hop {
  PropertiesAnalysis::PropertiesAnalysis() noexcept
    : plausibility_(0.0) {

  }

  void PropertiesAnalysis::analyse(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept {
    plausibility_ = 0.0;

    analyseImplementation(optimisationProblem);
  }

  double PropertiesAnalysis::getPlausibility() const noexcept {
    return plausibility_;
  }
}
