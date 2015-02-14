namespace mant {
  template <typename ParameterType>
  class LipschitzContinuityProperty : public ContinuityProperty<ParameterType> {
    public:
      using ContinuityProperty<ParameterType>::ContinuityProperty;

      bool isLipschitzContinuous() const noexcept;

      void isLipschitzContinuous(
          const bool isLipschitzContinuous) noexcept;

      double getLipschitzConstant() const noexcept;

      void setLipschitzConstant(
          const double lipschitzConstant) noexcept;

    protected:
      bool isLipschitzContinuous_;

      double lipschitzConstant_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  bool LipschitzContinuityProperty<ParameterType>::isLipschitzContinuous() const noexcept {
    return isLipschitzContinuous_;
  }

  template <typename ParameterType>
  void LipschitzContinuityProperty<ParameterType>::isLipschitzContinuous(
      const bool isLipschitzContinuous) noexcept {
    isLipschitzContinuous_ = isLipschitzContinuous;
  }

  template <typename ParameterType>
  double LipschitzContinuityProperty<ParameterType>::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }

  template <typename ParameterType>
  void LipschitzContinuityProperty<ParameterType>::setLipschitzConstant(
      const double lipschitzConstant) noexcept {
    lipschitzConstant_ = lipschitzConstant;
  }
}
