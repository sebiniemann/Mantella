namespace mant {
  template <typename ParameterType>
  class PassivePropertyAnalysis : public PropertyAnalysis<ParameterType> {
    public:
      using PropertyAnalysis<ParameterType>::PropertyAnalysis;

      using PropertyAnalysis<ParameterType>::analyse;

      void analyse(
          const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept;
    protected:
      using PropertyAnalysis<ParameterType>::analyseImplementation;

      virtual void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept = 0;
      virtual void analyseImplementation(
          const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  void PassivePropertyAnalysis<ParameterType>::analyse(
      const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept {
    PropertyAnalysis<ParameterType>::plausibility_ = 0.0;

    analyseImplementation(parameterToObjectiveValueMapping);
  }
}
