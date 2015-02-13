namespace mant {
  template <typename ParameterType>
  class DirectLipschitzContinuityAnalysis : public LipschitzContinuityAnalysis<ParameterType> {
    public:
      using LipschitzContinuityAnalysis<ParameterType>::LipschitzContinuityAnalysis;

    protected:
      void analyseImplementation(
          const std::unordered_map<arma::Col<ParameterType>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  void DirectLipschitzContinuityAnalysis<ParameterType>::analyseImplementation(
      const std::unordered_map<arma::Col<ParameterType>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept {
    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<ParameterType>& parameter = n->first;
      const double& objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        this->lipschitzConstant_ = std::max(this->lipschitzConstant_, std::abs(k->second - objectiveValue) / this->distanceFunction_.getDistance(parameter, k->first));
      }
    }
  }
}
