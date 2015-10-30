#include "mantella_bits/propertyAnalysis/passivePropertyAnalysis/fitnessDistanceCorrelationAnalysis.hpp"

// C++ standard library
#include <cassert>
#include <unordered_map>
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  void FitnessDistanceCorrelationAnalysis::analyseImplementation() {
    assert(samples_.size() > 1);

    arma::Mat<double> parameters(numberOfDimensions_, samples_.size());
    arma::Row<double> objectiveValues(parameters.n_cols);

    arma::uword n = 0;
    for (const auto& parameterToObjectiveValueMapping : samples_) {
      parameters.col(n) = parameterToObjectiveValueMapping.first;
      objectiveValues(n) = parameterToObjectiveValueMapping.second;
      ++n;
    }

    arma::uword bestParameterIndex;
    objectiveValues.min(bestParameterIndex);

    parameters.each_col() -= parameters.col(bestParameterIndex);

    parameters.shed_col(bestParameterIndex);
    objectiveValues.shed_col(bestParameterIndex);

    correlationCoefficient_ = arma::as_scalar(arma::cor(arma::sqrt(arma::sum(arma::square(parameters))), objectiveValues));
  }

  double FitnessDistanceCorrelationAnalysis::getCorrelationCoefficient() const {
    return correlationCoefficient_;
  }

  std::string FitnessDistanceCorrelationAnalysis::toString() const {
    return "fitness_distance_correlation_analysis";
  }
}
