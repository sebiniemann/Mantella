namespace mant {
  template <typename ParameterType>
  class PassivePropertyAnalysis : public PropertyAnalysis<ParameterType> {
    public:
      using PropertyAnalysis<ParameterType>::PropertyAnalysis;

      void analyse(
          const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMapping) noexcept;

    protected:
      virtual void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  void PassivePropertyAnalysis<ParameterType>::analyse(
      const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMapping) noexcept {
    analyseImplementation(parameterToObjectiveValueMapping);
  }
}
