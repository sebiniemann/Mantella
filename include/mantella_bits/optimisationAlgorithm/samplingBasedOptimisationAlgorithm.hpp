namespace mant {
  template <typename T = double>
  class SamplingBasedOptimisationAlgorithm : public OptimisationAlgorithm<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      using OptimisationAlgorithm<T>::OptimisationAlgorithm;
  };
}
