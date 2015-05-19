namespace mant {
  template <typename T>
  class DirectLipschitzContinuityAnalysis : public LipschitzContinuityAnalysis<T> {
    public:
      using LipschitzContinuityAnalysis<T>::LipschitzContinuityAnalysis;

    protected:
      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash, IsEqual>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  void DirectLipschitzContinuityAnalysis<T>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, double, Hash, IsEqual>& parameterToObjectiveValueMappings) noexcept {
    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<T>& parameter = n->first;
      const double& objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        this->lipschitzConstant_ = std::max(this->lipschitzConstant_, std::abs(k->second - objectiveValue) / this->distanceFunction_->getDistance(parameter, k->first));
      }
    }
  }
}
