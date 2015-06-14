namespace mant {
  template <typename T = double>
  class FitnessDistanceCorrelationAnalysis : public PassivePropertyAnalysis<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      double getCorrelationCoefficient() const noexcept;

      std::string toString() const noexcept override;

    protected:
      double correlationCoefficient_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  double FitnessDistanceCorrelationAnalysis<T>::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }

  template <typename T>
  void FitnessDistanceCorrelationAnalysis<T>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    assert(parameterToObjectiveValueMappings.size() > 1);
    
    arma::Mat<T> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem, parameterToObjectiveValueMappings.size());
    arma::Col<double> objectiveValues(parameters.n_cols);

    std::size_t n = 0;
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

  template <typename T>
  std::string FitnessDistanceCorrelationAnalysis<T>::toString() const noexcept {
    return "fitness_distance_correlation_analysis";
  }
}
