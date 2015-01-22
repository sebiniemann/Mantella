namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class ActivePropertiesAnalysis : public PropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using PropertiesAnalysis<ParameterType, DistanceFunction>::PropertiesAnalysis;
  };
}
