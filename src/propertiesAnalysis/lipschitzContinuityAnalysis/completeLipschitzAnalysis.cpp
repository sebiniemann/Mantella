#include <hop_bits/propertiesAnalysis/lipschitzContinuityAnalysis/completeLipschitzAnalysis.hpp>

// C++ Standard Library
#include <cstdlib>
#include <algorithm>

namespace hop {
  void CompleteLipschitzAnalysis::analyseImplementation(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) {
    double lipschitzConstant = 0;

    std::unordered_map<arma::Col<double>, double, CacheHasher, CacheKeyEqual> parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend(); ++n) {
      arma::Col<double> parameter = n->first;
      double objectiveValue = n->second;
      for (auto k = parameterToObjectiveValueMappings.cbegin(); k != parameterToObjectiveValueMappings.cend(); ++k) {
        lipschitzConstant = std::max(lipschitzConstant, std::abs((k->second - objectiveValue) / arma::norm(k->first - parameter)));
      }
    }

    lipschitzConstant_ = lipschitzConstant;
  }
}
