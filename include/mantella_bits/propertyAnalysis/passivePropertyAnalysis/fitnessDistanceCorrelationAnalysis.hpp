namespace mant {
  template <typename T, typename U = double>
  class FitnessDistanceCorrelationAnalysis : public PassivePropertyAnalysis<T, U> {
    static_assert(arma::is_supported_elem_type<T>::value, "The parameter type T must be supported by Armadillo C++, i.e. an arithmetic types, except bool.");
    static_assert(std::is_floating_point<U>::value, "U must be a floating point type.");
    
    public:
      using PassivePropertyAnalysis<T, U>::PassivePropertyAnalysis;

      double getCorrelationCoefficient() const noexcept;

      std::string toString() const noexcept override;

    protected:
      double correlationCoefficient_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  double FitnessDistanceCorrelationAnalysis<T, U>::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }

  template <typename T, typename U>
  void FitnessDistanceCorrelationAnalysis<T, U>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    assert(parameterToObjectiveValueMappings.size() > 1);
    
    arma::Mat<T> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem, parameterToObjectiveValueMappings.size());
    arma::Col<U> objectiveValues(parameters.n_cols);

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

  template <typename T, typename U>
  std::string FitnessDistanceCorrelationAnalysis<T, U>::toString() const noexcept {
    return "fitness_distance_correlation_analysis";
  }
}
