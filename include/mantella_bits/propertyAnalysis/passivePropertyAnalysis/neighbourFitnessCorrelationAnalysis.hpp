namespace mant {
  template <typename T, typename U = double>
  class NeighbourFitnessCorrelationAnalysis : public PassivePropertyAnalysis<T, U> {
    public:
      using PassivePropertyAnalysis<T, U>::PassivePropertyAnalysis;

      void setMaximalNeighbourDistance(
          const T maximalNeighbourDistance);
      
      double getCorrelationCoefficient() const noexcept;

      std::string toString() const noexcept override;

    protected:
      T maximalNeighbourDistance_;
    
      double correlationCoefficient_;

      inline void analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  void NeighbourFitnessCorrelationAnalysis<T, U>::setMaximalNeighbourDistance(
      const T maximalNeighbourDistance) {
    verify(maximalNeighbourDistance > 0, "");
      
    maximalNeighbourDistance_ = maximalNeighbourDistance;
  }

  template <typename T, typename U>
  double NeighbourFitnessCorrelationAnalysis<T, U>::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }

  template <typename T, typename U>
  void NeighbourFitnessCorrelationAnalysis<T, U>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    arma::Mat<T> parameters(parameterToObjectiveValueMappings.cbegin()->first.n_elem, parameterToObjectiveValueMappings.size());
    arma::Col<U> objectiveValues(parameterToObjectiveValueMappings.size());

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
  std::string NeighbourFitnessCorrelationAnalysis<T, U>::toString() const noexcept {
    return "neighbour_fitness_correlation_analysis";
  }
}
