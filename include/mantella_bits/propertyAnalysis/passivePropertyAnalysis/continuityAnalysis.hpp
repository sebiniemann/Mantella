namespace mant {
  template <typename ParameterType>
  class ContinuityAnalysis : public PassivePropertyAnalysis<ParameterType> {
    public:
      using PassivePropertyAnalysis<ParameterType>::PassivePropertyAnalysis;
  };
}
