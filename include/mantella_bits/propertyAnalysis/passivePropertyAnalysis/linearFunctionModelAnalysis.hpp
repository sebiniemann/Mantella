namespace mant {
  template <typename T = double>
  class LinearFunctionModelAnalysis : public PassivePropertyAnalysis<T> {
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
  arma::Col<double> LinearFunctionModelAnalysis<T>::getCoefficients() const noexcept {
    return coefficients_;
  }

  template <typename T>
  void LinearFunctionModelAnalysis<T>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    assert(parameterToObjectiveValueMappings.size() > 1);
    
    arma::Mat<T> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem + 1, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameters.n_cols);

    std::size_t n = 0;
    for(const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
        parameters.submat(0, n, parameters.n_rows - 2, n) = parameterToObjectiveValueMapping.first;
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
  std::string LinearFunctionModelAnalysis<T>::toString() const noexcept {
    return "linear_function_model_analysis";
  }
}
