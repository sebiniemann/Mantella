namespace mant {
  template <typename T>
  class PassivePropertyAnalysis : public PropertyAnalysis<T> {
    public:
      using PropertyAnalysis<T>::PropertyAnalysis;

      void analyse(
          const std::unordered_map<arma::Col<double>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMapping) noexcept;

    protected:
      virtual void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename T>
  void PassivePropertyAnalysis<T>::analyse(
      const std::unordered_map<arma::Col<double>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMapping) noexcept {
    analyseImplementation(parameterToObjectiveValueMapping);
  }
}
