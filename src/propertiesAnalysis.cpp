#include <hop_bits/propertiesAnalysis.hpp>

// C++ Standard Library
#include <cstdlib>

namespace hop {
  void PropertiesAnalysis::analyse(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) {
    arma::Mat<double> parameters(optimisationProblem->getCachedObjectiveValues().size(), optimisationProblem->getNumberOfDimensions());
    arma::Col<double> objectiveValues(optimisationProblem->getNumberOfDimensions());

    std::size_t n = 0;
    for (const auto& entry : optimisationProblem->getCachedObjectiveValues()) {
      parameters.col(n) = entry.first;
      objectiveValues.at(n) = entry.second;

      ++n;
    }

    belief_ = arma::datum::nan;
    plausibility_ = arma::datum::nan;

    analyseImplementation(parameters, objectiveValues);
  }

  double PropertiesAnalysis::getBelief() const {
    return belief_;
  }

  double PropertiesAnalysis::getPlausibility() const {
    return plausibility_;
  }
}
