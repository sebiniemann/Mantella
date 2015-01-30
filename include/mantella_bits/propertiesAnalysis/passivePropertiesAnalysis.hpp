namespace mant {
  template <typename ParameterType>
  class PassivePropertiesAnalysis : public PropertiesAnalysis<ParameterType> {
    public:
      using PropertiesAnalysis<ParameterType>::PropertiesAnalysis;

      using PropertiesAnalysis<ParameterType>::analyse;

      void analyse(
          const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept;
    protected:
      using PropertiesAnalysis<ParameterType>::analyseImplementation;

      virtual void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept = 0;
      virtual void analyseImplementation(
          const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  void PassivePropertiesAnalysis<ParameterType>::analyse(
      const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept {
    PropertiesAnalysis<ParameterType>::plausibility_ = 0.0;

    analyseImplementation(parameterToObjectiveValueMapping);
  }
}
