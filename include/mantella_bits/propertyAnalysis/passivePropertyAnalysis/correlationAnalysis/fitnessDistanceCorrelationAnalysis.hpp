namespace mant {
  class FitnessDistanceCorrelationAnalysis : public CorrelationAnalysis<double> {
    public:
      using CorrelationAnalysis::CorrelationAnalysis;

    protected:
      inline void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  inline void FitnessDistanceCorrelationAnalysis::analyseImplementation(
      const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept {
    arma::Mat<double> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

    unsigned int n = 0;
    for (const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
      parameters.col(n) = parameterToObjectiveValueMapping.first;
      objectiveValues.at(n) = parameterToObjectiveValueMapping.second;
      ++n;
    }

    unsigned int bestParameterIndex;
    objectiveValues.min(bestParameterIndex);

    parameters.each_col() -= parameters.col(bestParameterIndex);

    property_.setCorrelationCoefficient(arma::as_scalar(arma::cor(arma::sqrt(arma::sum(arma::square(parameters))), objectiveValues)));
  }
}
