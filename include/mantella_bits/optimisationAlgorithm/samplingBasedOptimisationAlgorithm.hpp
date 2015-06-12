namespace mant {
  template <typename T, typename U = double>
  class SamplingBasedOptimisationAlgorithm : public OptimisationAlgorithm<T, U> {
    static_assert(arma::is_supported_elem_type<T>::value, "The parameter type T must be supported by Armadillo C++, i.e. an arithmetic types, except bool.");
    static_assert(std::is_arithmetic<U>::value, "U must be an arithmetic type.");
    
    public:
      using OptimisationAlgorithm<T, U>::OptimisationAlgorithm;
  };
}
