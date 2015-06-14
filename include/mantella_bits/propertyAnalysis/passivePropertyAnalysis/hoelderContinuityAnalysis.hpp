namespace mant {
  template <typename T = double>
  class HoelderContinuityAnalysis : public PassivePropertyAnalysis<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      double getHoelderExponent() const noexcept;

      std::string toString() const noexcept override;
      
    protected:
      double hoelderExponent_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  double HoelderContinuityAnalysis<T>::getHoelderExponent() const noexcept {
    return hoelderExponent_;
  }

  template <typename T>
  void HoelderContinuityAnalysis<T>::analyseImplementation(
          const std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    assert(parameterToObjectiveValueMappings.size() > 1);
    
    hoelderExponent_= std::numeric_limits<double>::max();
    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<T>& parameter = n->first;
      const T objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        hoelderExponent_ = std::min(hoelderExponent_, std::log(k->second - objectiveValue) / std::log(static_cast<double>(arma::norm(k->first - parameter)))); 
      }
    }
  }

  template <typename T>
  std::string HoelderContinuityAnalysis<T>::toString() const noexcept {
    return "hoelder_continuity_analysis";
  }
}
