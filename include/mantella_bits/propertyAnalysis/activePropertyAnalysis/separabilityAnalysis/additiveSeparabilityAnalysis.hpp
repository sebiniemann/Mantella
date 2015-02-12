namespace mant {
  template <typename ParameterType>
  class AdditiveSeparabilityAnalysis : public SeparabilityAnalysis<ParameterType> {
    public:
      explicit AdditiveSeparabilityAnalysis() noexcept;

      void setAdditiveSeparabilityMedianErrorThreshold(
          const double& additiveSeparabilityMedianErrorThreshold);

      std::vector<arma::Col<unsigned int>> getSeparation() const noexcept;

      bool isSeparable() const noexcept;

    protected:
      std::vector<arma::Col<unsigned int>> separation_;
      bool isSeparable_;

      double additiveSeparabilityMedianErrorThreshold_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  AdditiveSeparabilityAnalysis<ParameterType>::AdditiveSeparabilityAnalysis() noexcept {
    setAdditiveSeparabilityMedianErrorThreshold(0.75);
  }

  template <typename ParameterType>
  void AdditiveSeparabilityAnalysis<ParameterType>::setAdditiveSeparabilityMedianErrorThreshold(
      const double& additiveSeparabilityMedianErrorThreshold) {
    if(additiveSeparabilityMedianErrorThreshold < 0 || additiveSeparabilityMedianErrorThreshold > 1) {
      throw std::logic_error("The additive separability error threshold (" + std::to_string(additiveSeparabilityMedianErrorThreshold) + ") must be within 0 and 1.");
    }

    additiveSeparabilityMedianErrorThreshold_ = additiveSeparabilityMedianErrorThreshold;
  }
}
