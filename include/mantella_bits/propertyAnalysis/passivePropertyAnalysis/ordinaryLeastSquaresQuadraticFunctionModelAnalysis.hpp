namespace mant {
  template <typename T>
  class OrdinaryLeastSquaresQuadraticFunctionModelAnalysis : public PassivePropertyAnalysis<T> {
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      arma::Col<double> getCoefficients() const noexcept;

    protected:
      arma::Col<double> coefficients_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  arma::Col<double> OrdinaryLeastSquaresQuadraticFunctionModelAnalysis<T>::getCoefficients() const noexcept {
    return coefficients_;
  }

  template <typename T>
  void OrdinaryLeastSquaresQuadraticFunctionModelAnalysis<T>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    arma::Mat<T> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem * (parameterToObjectiveValueMappings.cbegin()->first.n_elem + 3) / 2 + 1, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

    std::size_t n = 0;
    for (const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
      const arma::Col<T>& parameter = parameterToObjectiveValueMapping.first;

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
      coefficients_ = (parameters * parameters.t()).i() * parameters * objectiveValues;
    } catch (...) {
      coefficients_ = arma::pinv(parameters * parameters.t()) * parameters * objectiveValues;
    }
  }
}
