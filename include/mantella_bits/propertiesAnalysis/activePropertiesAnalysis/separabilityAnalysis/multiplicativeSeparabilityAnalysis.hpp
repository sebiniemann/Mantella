namespace mant {
  template <typename ParameterType>
  class MultiplicativeSeparabilityAnalysis : public SeparabilityAnalysis<ParameterType> {
    public:
      explicit MultiplicativeSeparabilityAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      MultiplicativeSeparabilityAnalysis(const MultiplicativeSeparabilityAnalysis&) = delete;
      MultiplicativeSeparabilityAnalysis& operator=(const MultiplicativeSeparabilityAnalysis&) = delete;

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
  MultiplicativeSeparabilityAnalysis<ParameterType>::MultiplicativeSeparabilityAnalysis() noexcept {
    setAdditiveSeparabilityMedianErrorThreshold(0.75);
  }

  template <typename ParameterType>
  void MultiplicativeSeparabilityAnalysis<ParameterType>::setAdditiveSeparabilityMedianErrorThreshold(
      const double& additiveSeparabilityMedianErrorThreshold) {
    if(additiveSeparabilityMedianErrorThreshold < 0 || additiveSeparabilityMedianErrorThreshold > 1) {
      throw std::logic_error("The additive separability error threshold (" + std::to_string(additiveSeparabilityMedianErrorThreshold) + ") must be within 0 and 1.");
    }

    additiveSeparabilityMedianErrorThreshold_ = additiveSeparabilityMedianErrorThreshold;
  }
}
