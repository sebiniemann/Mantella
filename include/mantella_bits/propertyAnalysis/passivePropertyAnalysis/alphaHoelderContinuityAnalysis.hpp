namespace mant {
  template <typename T, typename U = double>
  class AlphaHoelderContinuityAnalysis : public PassivePropertyAnalysis<T> {
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      double getAlpha() const noexcept;
      double getLipschitzConstant() const noexcept;

    protected:
      double alpha_;
      double lipschitzConstant_;

      void analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  double AlphaHoelderContinuityAnalysis<T>::getAlpha() const noexcept {
    return alpha_;
  }

  template <typename T>
  double AlphaHoelderContinuityAnalysis<T>::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }

  template <typename T>
  void AlphaHoelderContinuityAnalysis<T>::analyseImplementation(
          const std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>>& parameterToObjectiveValueMappings) noexcept {
    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<T>& parameter = n->first;
      const double& objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        if()
          lipschitzConstant_ = std::max(lipschitzConstant_, std::abs(k->second - objectiveValue) / this->distanceFunction_->getDistance(parameter, k->first));
        } else {
          lipschitzConstant_ = std::max(lipschitzConstant_, std::abs(k->second - objectiveValue) / arma::norm(k->first - parameter)); 
        }
      }
    }
  }
}
