namespace mant {
  template <typename T, typename U = double>
  class LipschitzContinuityAnalysis : public PassivePropertyAnalysis<T, U> {
    public:
      using PassivePropertyAnalysis<T, U>::PassivePropertyAnalysis;

      double getLipschitzConstant() const noexcept;

      std::string toString() const noexcept override;

    protected:
      double lipschitzConstant_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  double LipschitzContinuityAnalysis<T, U>::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }

  template <typename T, typename U>
  void LipschitzContinuityAnalysis<T, U>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<T>& parameter = n->first;
      const U objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
      }
    }
  }

  template <typename T, typename U>
  std::string LipschitzContinuityAnalysis<T, U>::toString() const noexcept {
    return "lipschitz_continuity_analysis";
  }
}
