namespace mant {
  class OrdinaryLeastSquaresLinearFunctionModelAnalysis : public LinearFunctionModelAnalysis {
    public:
      using LinearFunctionModelAnalysis::LinearFunctionModelAnalysis;

    protected:
      inline void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  inline void OrdinaryLeastSquaresLinearFunctionModelAnalysis::analyseImplementation(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& parameterToObjectiveValueMappings) noexcept {
    arma::Mat<double> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem + 1, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

    std::size_t n = 0;
    for(const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
        parameters.submat(0, n, parameters.n_rows - 2, n) = parameterToObjectiveValueMapping.first;
        objectiveValues.at(n) = parameterToObjectiveValueMapping.second;
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
