namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class ProportionalityAnalysis : public ActivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using ActivePropertiesAnalysis<ParameterType, DistanceFunction>::ActivePropertiesAnalysis;

      void setMaximalNumberOfIterations(
          const unsigned int& maximalNumberOfIterations) noexcept;

    protected:
      unsigned int maximalNumberOfIterations_;
  };

  //
  // Implementation
  //

  template <typename ParameterType, class DistanceFunction>
  void ProportionalityAnalysis<ParameterType, DistanceFunction>::setMaximalNumberOfIterations(
      const unsigned int& maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }
}
