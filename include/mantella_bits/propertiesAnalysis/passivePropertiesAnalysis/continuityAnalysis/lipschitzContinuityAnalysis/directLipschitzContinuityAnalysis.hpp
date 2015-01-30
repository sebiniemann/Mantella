namespace mant {
  template <typename ParameterType>
  class DirectLipschitzContinuityAnalysis : public LipschitzContinuityAnalysis<ParameterType> {
    public:
      using LipschitzContinuityAnalysis<ParameterType>::LipschitzContinuityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::unordered_map<arma::Col<ParameterType>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept override;
  };

  template <>
  inline void DirectLipschitzContinuityAnalysis<double>::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept;

  //
  // Implementation
  //

  template <typename ParameterType>
  void DirectLipschitzContinuityAnalysis<ParameterType>::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept {
    this->lipschitzConstant_ = 0.0;

    const std::unordered_map<arma::Col<ParameterType>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<ParameterType>& parameter = n->first;
      const double& objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        this->lipschitzConstant_ = std::max(this->lipschitzConstant_, std::abs(k->second - objectiveValue) / this->distanceFunction_.getDistance(parameter, k->first));
      }
    }
  }

  template <typename ParameterType>
  void DirectLipschitzContinuityAnalysis<ParameterType>::analyseImplementation(
      const std::unordered_map<arma::Col<ParameterType>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept {

  }

  template <typename ParameterType>
  void DirectLipschitzContinuityAnalysis<ParameterType>::analyseImplementation(
      const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept {

  }

  template <>
  inline void DirectLipschitzContinuityAnalysis<double>::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept {
    lipschitzConstant_ = 0.0;

    const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<double>& parameter = n->first;
      const double& objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        lipschitzConstant_ = std::max(lipschitzConstant_, std::abs(k->second - objectiveValue) / arma::norm(k->first - parameter));
      }
    }
  }
}
