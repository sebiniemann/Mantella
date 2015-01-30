namespace mant {
  template <typename ParameterType>
  class SamplingBasedAlgorithm : public OptimisationAlgorithm<ParameterType> {
    public:
      using OptimisationAlgorithm<ParameterType>::OptimisationAlgorithm;
  };
}
