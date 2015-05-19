namespace mant {
  template <typename T>
  class LipschitzContinuityProperty : public ContinuityProperty<T> {
    public:
      using ContinuityProperty<T>::ContinuityProperty;

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

  template <typename T>
  bool LipschitzContinuityProperty<T>::isLipschitzContinuous() const noexcept {
    return isLipschitzContinuous_;
  }

  template <typename T>
  void LipschitzContinuityProperty<T>::isLipschitzContinuous(
      const bool isLipschitzContinuous) noexcept {
    isLipschitzContinuous_ = isLipschitzContinuous;
  }

  template <typename T>
  double LipschitzContinuityProperty<T>::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }

  template <typename T>
  void LipschitzContinuityProperty<T>::setLipschitzConstant(
      const double lipschitzConstant) noexcept {
    lipschitzConstant_ = lipschitzConstant;
  }
}
