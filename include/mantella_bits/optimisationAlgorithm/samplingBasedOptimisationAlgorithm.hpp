namespace mant {
  template <typename T, typename U = double>
  class SamplingBasedOptimisationAlgorithm : public OptimisationAlgorithm<T, U> {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type.");
    static_assert(std::is_arithmetic<U>::value, "U must be an arithmetic type.");
    
    public:
      using OptimisationAlgorithm<T, U>::OptimisationAlgorithm;
  };
}
