namespace mant {
  template <typename T = double>
  class LipschitzContinuityAnalysis : public PassivePropertyAnalysis<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      double getLipschitzConstant() const noexcept;

      std::string toString() const noexcept override;

    protected:
      double lipschitzConstant_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  double LipschitzContinuityAnalysis<T>::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }

  template <typename T>
  void LipschitzContinuityAnalysis<T>::analyseImplementation(
      const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    assert(parameterToObjectiveValueMappings.size() > 1);
    
    lipschitzConstant_ = 0.0;
    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<T>& parameter = n->first;
      const T objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        lipschitzConstant_ = std::max(lipschitzConstant_, std::abs(k->second - objectiveValue) / arma::norm(k->first - parameter)); 
      }
    }
  }

  template <typename T>
  std::string LipschitzContinuityAnalysis<T>::toString() const noexcept {
    return "lipschitz_continuity_analysis";
  }
}
