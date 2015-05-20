namespace mant {
  template <typename T>
  class OrdinaryLeastSquaresLinearFunctionModelAnalysis : public PassivePropertyAnalysis<T> {
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
  arma::Col<double> OrdinaryLeastSquaresLinearFunctionModelAnalysis<T>::getCoefficients() const noexcept {
    return coefficients_;
  }

  template <typename T>
  void OrdinaryLeastSquaresLinearFunctionModelAnalysis<T>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    arma::Mat<T> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem + 1, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

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
}
