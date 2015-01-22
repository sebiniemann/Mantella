namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class ContinuityAnalysis : public PassivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using PassivePropertiesAnalysis<ParameterType, DistanceFunction>::PassivePropertiesAnalysis;
  };
}
