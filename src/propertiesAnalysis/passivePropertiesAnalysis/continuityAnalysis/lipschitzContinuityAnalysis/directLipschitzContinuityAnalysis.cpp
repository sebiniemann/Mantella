#include <mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis/directLipschitzContinuityAnalysis.hpp>

// C++ Standard Library
#include <cstdlib>
#include <algorithm>

namespace mant {
  void DirectLipschitzContinuityAnalysis::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept {
    lipschitzConstant_ = 0.0;

    const std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual>& parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<double>& parameter = n->first;
      const double& objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        lipschitzConstant_ = std::max(lipschitzConstant_, std::abs(k->second - objectiveValue) / arma::norm(k->first - parameter));
      }
    }
  }

  void DirectLipschitzContinuityAnalysis::analyseImplementation(
      const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept {

  }
}
