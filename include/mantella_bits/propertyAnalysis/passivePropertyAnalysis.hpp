namespace mant {
  template <typename ParameterType>
  class PassivePropertyAnalysis : public PropertyAnalysis<ParameterType> {
    public:
      using PropertyAnalysis<ParameterType>::PropertyAnalysis;

      void analyse(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& parameterToObjectiveValueMapping) noexcept;

    protected:
      virtual void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& parameterToObjectiveValueMappings) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  void PassivePropertyAnalysis<ParameterType>::analyse(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& parameterToObjectiveValueMapping) noexcept {
    analyseImplementation(parameterToObjectiveValueMapping);
  }
}
