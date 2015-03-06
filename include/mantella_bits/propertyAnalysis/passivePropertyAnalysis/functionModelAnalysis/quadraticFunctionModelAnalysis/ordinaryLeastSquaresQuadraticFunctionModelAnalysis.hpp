namespace mant {
  class OrdinaryLeastSquaresQuadraticFunctionModelAnalysis : public QuadraticFunctionModelAnalysis {
    public:
      using QuadraticFunctionModelAnalysis::QuadraticFunctionModelAnalysis;

    protected:
      inline void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  inline void OrdinaryLeastSquaresQuadraticFunctionModelAnalysis::analyseImplementation(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& parameterToObjectiveValueMappings) noexcept {
    arma::Mat<double> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem * (parameterToObjectiveValueMappings.cbegin()->first.n_elem + 3) / 2 + 1, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

    std::size_t n = 0;
    for (const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
      const arma::Col<double>& parameter = parameterToObjectiveValueMapping.first;

      parameters.submat(0, n, parameter.n_elem - 1, n) = parameter;
      parameters.submat(parameter.n_elem, n, 2 * parameter.n_elem - 1,  n) = arma::square(parameter);

      std::size_t k = 2 * parameter.n_elem;
      for (std::size_t l = 0; l < parameter.n_elem; ++l) {
        for (std::size_t m = l + 1; m < parameter.n_elem; ++m) {
          parameters(k++, n) = parameter(l) * parameter(m);
        }
      }

      objectiveValues(n) = parameterToObjectiveValueMapping.second;
      ++n;
    }
    parameters.row(parameters.n_rows - 1).fill(1.0);

    try {
      property_.setCoefficients((parameters * parameters.t()).i() * parameters * objectiveValues);
    } catch (...) {
      property_.setCoefficients(arma::pinv(parameters * parameters.t()) * parameters * objectiveValues);
    }
  }
}
