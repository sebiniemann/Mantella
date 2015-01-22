namespace mant {
  class LinearOrdinaryLeastSquares : public LinearModelAnalysis<double, EuclideanDistance> {
    public:
      using LinearModelAnalysis<double, EuclideanDistance>::LinearModelAnalysis;

    protected:
      inline void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
      inline void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept override;
      inline void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept override;
  };

  inline void LinearOrdinaryLeastSquares::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept {
    const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    optimisationProblem->getObjectiveValue(arma::ones<arma::Col<double>>(optimisationProblem->getNumberOfDimensions()));

    arma::Mat<double> parameters(optimisationProblem->getNumberOfDimensions() + 1, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

    std::size_t n = 0;
    for(const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
        parameters.submat(0, n, parameters.n_rows - 2, n) = parameterToObjectiveValueMapping.first;
        objectiveValues.at(n) = parameterToObjectiveValueMapping.second;
        ++n;
    }
    parameters.row(parameters.n_rows - 1).fill(1.0);

    try {
      linearModelEstimator_ = (parameters * parameters.t()).i() * parameters * objectiveValues;
      residuals_ = objectiveValues - parameters.t() * linearModelEstimator_;
    } catch (const std::runtime_error& exception ) {
      linearModelEstimator_ = {};
      residuals_ = {};
    }
  }

  inline void LinearOrdinaryLeastSquares::analyseImplementation(
      const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept {

  }

  inline void LinearOrdinaryLeastSquares::analyseImplementation(
      const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept {

  }
}
