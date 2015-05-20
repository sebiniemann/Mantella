namespace mant {
  template <typename T>
  class LipschitzContinuityAnalysis : public PassivePropertyAnalysis<T> {
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      double getLipschitzConstant() const noexcept;

    protected:
      double lipschitzConstant_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  double LipschitzContinuityAnalysis<T>::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }

  template <typename T>
  void LipschitzContinuityAnalysis<T>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<T>& parameter = n->first;
      const double& objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        lipschitzConstant_ = std::max(lipschitzConstant_, std::abs(k->second - objectiveValue) / this->distanceFunction_->getDistance(parameter, k->first));
      }
    }
  }
}
