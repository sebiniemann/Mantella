#include <hop_bits/propertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis/directLipschitzContinuityAnalysis.hpp>

// C++ Standard Library
#include <cstdlib>
#include <algorithm>

namespace hop {
  void DirectLipschitzContinuityAnalysis::analyseImplementation(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) {
    lipschitzConstant_ = 0;

    std::unordered_map<arma::Col<double>, double, CacheHasher, CacheKeyEqual> parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      arma::Col<double> parameter = n->first;
      double objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        lipschitzConstant_ = std::max(lipschitzConstant_, std::abs(k->second - objectiveValue) / arma::norm(k->first - parameter));
      }
    }
  }
}
