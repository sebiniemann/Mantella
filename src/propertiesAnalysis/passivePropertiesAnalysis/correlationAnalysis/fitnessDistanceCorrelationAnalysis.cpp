#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis/fitnessDistanceCorrelationAnalysis.hpp>

// C++ Standard Library
#include <cstdlib>
#include <algorithm>
#include <iostream>

namespace hop {
  void FitnessDistanceCorrelationAnalysis::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept {
    const std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual>& parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    arma::Mat<double> parameters(optimisationProblem->getNumberOfDimensions(), parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

    unsigned int n = 0;
    for (const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
      parameters.col(n) = parameterToObjectiveValueMapping.first;
      objectiveValues.at(n) = parameterToObjectiveValueMapping.second;
      ++n;
    }

    unsigned int bestParameterIndex;
    objectiveValues.min(bestParameterIndex);

    parameters.each_col() -= parameters.col(bestParameterIndex);

    correlationCoefficient_ = arma::as_scalar(arma::cor(arma::sqrt(arma::sum(arma::square(parameters))), objectiveValues));
  }

  void FitnessDistanceCorrelationAnalysis::analyseImplementation(
      const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept {

  }
}
