namespace mant {
  template <typename T, typename U = double>
  class HoelderContinuityAnalysis : public PassivePropertyAnalysis<T, U> {
    public:
      using PassivePropertyAnalysis<T, U>::PassivePropertyAnalysis;

      double getHoelderExponent() const noexcept;

      std::string toString() const noexcept override;
      
    protected:
      double hoelderExponent_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  double HoelderContinuityAnalysis<T, U>::getHoelderExponent() const noexcept {
    return hoelderExponent_;
  }

  template <typename T, typename U>
  void HoelderContinuityAnalysis<T, U>::analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<T>& parameter = n->first;
      const U objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        if (std::is_integral<T>::value) {
          hoelderExponent_ = std::min(hoelderExponent_, std::log(k->second - objectiveValue) / std::log(this->distanceFunction_->getDistance(parameter, k->first)));
        } else {
          hoelderExponent_ = std::min(hoelderExponent_, std::log(k->second - objectiveValue) / std::log(arma::norm(k->first - parameter))); 
        }
      }
    }
  }

  template <typename T, typename U>
  std::string HoelderContinuityAnalysis<T, U>::toString() const noexcept {
    return "hoelder_continuity_analysis";
  }
}
