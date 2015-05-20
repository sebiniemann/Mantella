namespace mant {
  template <typename T>
  class FitnessCloudCorrelationAnalysis : public PassivePropertyAnalysis<T> {
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      double getCorrelationCoefficient() const noexcept;

    protected:
      double correlationCoefficient_;

      inline void analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  double FitnessCloudCorrelationAnalysis<T>::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }

  template <typename T>
  void FitnessCloudCorrelationAnalysis<T>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    arma::Mat<T> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameterToObjectiveValueMappings.size());

    unsigned int n = 0;
    for (const auto& parameterToObjectiveValueMapping : parameterToObjectiveValueMappings) {
      parameters.col(n) = parameterToObjectiveValueMapping.first;
      objectiveValues(n) = parameterToObjectiveValueMapping.second;
      ++n;
    }

    unsigned int bestParameterIndex;
    objectiveValues.min(bestParameterIndex);

    parameters.each_col() -= parameters.col(bestParameterIndex);

    correlationCoefficient_ = arma::as_scalar(arma::cor(arma::sqrt(arma::sum(arma::square(parameters))), objectiveValues));
  }
}
