#include <mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis/quadraticOrdinaryLeastSquares.hpp>

namespace mant {
  void QuadraticOrdinaryLeastSquares::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept {
    const std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual>& parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    arma::Mat<double> parameters(optimisationProblem->getNumberOfDimensions() * (optimisationProblem->getNumberOfDimensions() + 3) / 2 + 1, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

    std::size_t n = 0;
    for (const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
      const arma::Col<double>& parameter = parameterToObjectiveValueMapping.first;

      parameters.submat(0, n, parameter.n_elem - 1, n) = parameter;
      parameters.submat(parameter.n_elem, n, 2 * parameter.n_elem - 1,  n) = arma::square(parameter);

      std::size_t k = 2 * parameter.n_elem;
      for (std::size_t l = 0; l < parameter.n_elem; ++l) {
        for (std::size_t m = l + 1; m < parameter.n_elem; ++m) {
          parameters.at(k++, n) = parameter.at(l) * parameter.at(m);
        }
      }

      objectiveValues.at(n) = parameterToObjectiveValueMapping.second;
      ++n;
    }
    parameters.row(parameters.n_rows - 1).fill(1.0);

    try {
      quadraticModelEstimator_ = (parameters * parameters.t()).i() * parameters * objectiveValues;
      residuals_ = objectiveValues - parameters.t() * quadraticModelEstimator_;
    } catch (const std::runtime_error& exception ) {
      quadraticModelEstimator_ = {};
      residuals_ = {};
    }
  }

  void QuadraticOrdinaryLeastSquares::analyseImplementation(
      const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept {

  }
}
