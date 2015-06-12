namespace mant {
  template <typename T, typename U = double>
  class SamplingBasedOptimisationAlgorithm : public OptimisationAlgorithm<T, U> {
    static_assert(arma::is_supported_elem_type<T>::value, "The parameter type T must be supported by Armadillo C++, i.e. an arithmetic types, except bool.");
    static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
    public:
      using OptimisationAlgorithm<T, U>::OptimisationAlgorithm;
  };
}
