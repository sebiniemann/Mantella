namespace mant {
  template <typename T, typename U = double>
  class PassivePropertyAnalysis : public PropertyAnalysis<T, U> {
    static_assert(arma::is_supported_elem_type<T>::value, "The parameter type T must be supported by Armadillo C++, i.e. an arithmetic types, except bool.");
    static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
    public:
      using PropertyAnalysis<T, U>::PropertyAnalysis;
      
      void analyse(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings);

    protected:
      virtual void analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  void PassivePropertyAnalysis<T, U>::analyse(
      const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) {
    verify(parameterToObjectiveValueMappings.size() > 1, "");
    
    analyseImplementation(parameterToObjectiveValueMappings);
  }
}
