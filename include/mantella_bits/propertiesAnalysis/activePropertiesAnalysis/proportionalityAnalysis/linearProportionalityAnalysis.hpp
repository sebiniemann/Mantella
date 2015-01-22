namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class LinearProportionalityAnalysis : public ProportionalityAnalysis<ParameterType, DistanceFunction> {
    public:
      using ProportionalityAnalysis<ParameterType, DistanceFunction>::ProportionalityAnalysis;
  };
}
