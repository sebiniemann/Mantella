namespace mant {
  template <typename T = double>
  class QuadraticFunctionModelAnalysis : public PassivePropertyAnalysis<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      arma::Col<double> getCoefficients() const noexcept;

      std::string toString() const noexcept override;

    protected:
      arma::Col<double> coefficients_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  arma::Col<double> QuadraticFunctionModelAnalysis<T>::getCoefficients() const noexcept {
    return coefficients_;
  }

  template <typename T>
  void QuadraticFunctionModelAnalysis<T>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    assert(parameterToObjectiveValueMappings.size() > 1);
    
    arma::Mat<T> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem * (parameterToObjectiveValueMappings.cbegin()->first.n_elem + 3) / 2 + 1, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameters.n_cols);
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
    parameters.row(parameters.n_rows - 1).fill(static_cast<T>(1.0L));

    try {
      coefficients_ = arma::conv_to<arma::Mat<double>>::from((parameters * parameters.t()).i() * parameters) * objectiveValues;
    } catch (...) {
      coefficients_ = arma::conv_to<arma::Mat<double>>::from(arma::pinv(parameters * parameters.t()) * parameters) * objectiveValues;
    }
  }

  template <typename T>
  std::string QuadraticFunctionModelAnalysis<T>::toString() const noexcept {
    return "quadratic_function_model_analysis";
  }
}
