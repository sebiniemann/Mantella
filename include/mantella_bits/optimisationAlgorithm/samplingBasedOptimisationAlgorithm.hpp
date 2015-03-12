namespace mant {
  template <typename ParameterType>
  class SamplingBasedOptimisationAlgorithm : public OptimisationAlgorithm<ParameterType> {
    public:
      using OptimisationAlgorithm<ParameterType>::OptimisationAlgorithm;
  };
}
