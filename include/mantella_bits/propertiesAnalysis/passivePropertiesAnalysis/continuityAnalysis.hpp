namespace mant {
  template <typename ParameterType>
  class ContinuityAnalysis : public PassivePropertiesAnalysis<ParameterType> {
    public:
      using PassivePropertiesAnalysis<ParameterType>::PassivePropertiesAnalysis;
  };
}
