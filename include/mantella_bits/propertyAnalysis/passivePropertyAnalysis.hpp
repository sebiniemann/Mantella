namespace mant {
  template <typename T = double>
  class PassivePropertyAnalysis : public PropertyAnalysis<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      using PropertyAnalysis<T>::PropertyAnalysis;
      
      void analyse(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings);

    protected:
      virtual void analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename T>
  void PassivePropertyAnalysis<T>::analyse(
      const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) {
    verify(parameterToObjectiveValueMappings.size() > 1, "");
    
    analyseImplementation(parameterToObjectiveValueMappings);
  }
}
