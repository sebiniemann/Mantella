namespace mant {
  template <typename T, typename U = double>
  class LinearFunctionModelAnalysis : public PassivePropertyAnalysis<T, U> {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type.");
    static_assert(std::is_floating_point<U>::value, "U must be a floating point type.");
    
    public:
      using PassivePropertyAnalysis<T, U>::PassivePropertyAnalysis;

      arma::Col<double> getCoefficients() const noexcept;

      std::string toString() const noexcept override;

    protected:
      arma::Col<double> coefficients_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  arma::Col<double> LinearFunctionModelAnalysis<T, U>::getCoefficients() const noexcept {
    return coefficients_;
  }

  template <typename T, typename U>
  void LinearFunctionModelAnalysis<T, U>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    arma::Mat<T> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem + 1, parameterToObjectiveValueMappings.size());
    arma::Col<U> objectiveValues(parameters.n_cols);

    std::size_t n = 0;
    for(const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
        parameters.submat(0, n, parameters.n_rows - 2, n) = parameterToObjectiveValueMapping.first;
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

  template <typename T, typename U>
  std::string LinearFunctionModelAnalysis<T, U>::toString() const noexcept {
    return "linear_function_model_analysis";
  }
}
