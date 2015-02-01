namespace mant {
  template <typename ParameterType>
  class SeparabilityAnalysis : public ActivePropertiesAnalysis<ParameterType> {
    public:
      explicit SeparabilityAnalysis() noexcept
        : maximalNumberOfIterations_(1000.0) {

      }

      void setMaximalNumberOfIterations(
          const unsigned int& maximalNumberOfIterations) noexcept;

    protected:
      unsigned int maximalNumberOfIterations_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  void SeparabilityAnalysis<ParameterType>::setMaximalNumberOfIterations(
      const unsigned int& maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }
}
