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
    
    arma::Col<U> objectiveValues(parameters.n_cols);
    arma::Mat<unsigned int> neighbours(parameters.n_cols, parameters.n_cols);
    
    std::size_t nn = 0;
    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
    
      const arma::Col<T>& parameter = n->first;
      objectiveValues(nn) = n->second;
      
      std::size_t kk = 0;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        if (std::is_integral<T>::value) {
          neighbours(nn, kk) = arma::find(this->distanceFunction_->getDistance(parameter, k->first) <= maximalNeighbourDistance_);
        } else {
          neighbours(nn, kk) = arma::find(arma::norm(k->first - parameter) <= maximalNeighbourDistance_); 
        }
        
        ++kk;
      }
      ++nn;
    }
    neighbours = arma::symmatu(neighbours);
    
    arma::Col<U> medianNeighbourObjectiveValues(objectiveValues.n_elem);
    for (std::size_t n = 0; n < objectiveValues.n_elem; ++n) {
      medianNeighbourObjectiveValues(n) = arma::median(objectiveValues.elem(neighbours.col(n)));
    }

    correlationCoefficient_ = arma::as_scalar(arma::cor(objectiveValues, medianNeighbourObjectiveValues));
  }

  template <typename T, typename U>
  std::string NeighbourFitnessCorrelationAnalysis<T, U>::toString() const noexcept {
    return "neighbour_fitness_correlation_analysis";
  }
}
