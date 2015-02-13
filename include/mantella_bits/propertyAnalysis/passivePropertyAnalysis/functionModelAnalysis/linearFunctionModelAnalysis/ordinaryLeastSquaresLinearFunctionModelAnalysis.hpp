namespace mant {
  class OrdinaryLeastSquaresLinearFunctionModelAnalysis : public LinearFunctionModelAnalysis<double> {
    public:
      using LinearFunctionModelAnalysis<double>::LinearFunctionModelAnalysis;

    protected:
      inline void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  inline void OrdinaryLeastSquaresLinearFunctionModelAnalysis::analyseImplementation(
      const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept {
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
      // TODO Avoid try catch (and search for others)
      linearModelEstimator_ = (parameters * parameters.t()).i() * parameters * objectiveValues;
      residuals_ = objectiveValues - parameters.t() * linearModelEstimator_;
    } catch (const std::runtime_error& exception ) {
      linearModelEstimator_ = {};
      residuals_ = {};
    }
  }
}
