#include <hop_bits/propertiesAnalysis/functionModelAnalysis/linearModelAnalysis/linearOrdinaryLeastSquares.hpp>

// HOP
#include <hop_bits/helper/regression.hpp>

namespace hop {
  void LinearOrdinaryLeastSquares::analyseImplementation(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) {
    std::unordered_map<arma::Col<double>, double, CacheHasher, CacheKeyEqual> parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    arma::Mat<double> parameters(optimisationProblem->getNumberOfDimensions() + 1, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

    std::size_t n = 0;
    for(const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
        parameters.submat(0, n, parameters.n_rows - 2, n) = parameterToObjectiveValueMapping.first;
        objectiveValues.at(n) = parameterToObjectiveValueMapping.second;
        ++n;
    }
    parameters.row(parameters.n_rows - 1).fill(1.0);

    linearModelEstimator_ = getOrdinaryLeastSquaresEstimator(parameters, objectiveValues);
    residuals_ = objectiveValues - parameters.t() * linearModelEstimator_;
  }
}
