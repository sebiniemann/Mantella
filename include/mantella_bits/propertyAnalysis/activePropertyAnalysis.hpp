namespace mant {
  template <typename ParameterType>
  class ActivePropertyAnalysis : public PropertyAnalysis<ParameterType> {
    public:
      using PropertyAnalysis<ParameterType>::PropertyAnalysis;
  };
}
