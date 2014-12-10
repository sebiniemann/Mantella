#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis.hpp>

namespace hop {
  void PassivePropertiesAnalysis::analyse(
      const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept {
    plausibility_ = 0.0;

    analyseImplementation(parameterToObjectiveValueMapping);
  }
}
