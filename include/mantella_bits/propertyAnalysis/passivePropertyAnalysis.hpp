namespace mant {
  template <typename T, typename U = double>
  class PassivePropertyAnalysis : public PropertyAnalysis<T, U> {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type.");
    static_assert(std::is_floating_point<U>::value, "U must be a floating point type.");
    
    public:
      using PropertyAnalysis<T, U>::PropertyAnalysis;

      void analyse(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMapping) noexcept;

    protected:
      virtual void analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  void PassivePropertyAnalysis<T, U>::analyse(
      const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMapping) noexcept {
    analyseImplementation(parameterToObjectiveValueMapping);
  }
}
