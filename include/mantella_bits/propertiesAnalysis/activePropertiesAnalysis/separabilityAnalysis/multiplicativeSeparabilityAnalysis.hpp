namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class MultiplicativeSeparabilityAnalysis : public SeparabilityAnalysis<ParameterType, DistanceFunction> {
    public:
      explicit MultiplicativeSeparabilityAnalysis() ;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      MultiplicativeSeparabilityAnalysis(const MultiplicativeSeparabilityAnalysis&) = delete;
      MultiplicativeSeparabilityAnalysis& operator=(const MultiplicativeSeparabilityAnalysis&) = delete;

      void setAdditiveSeparabilityMedianErrorThreshold(
          const double& additiveSeparabilityMedianErrorThreshold);

      std::vector<arma::Col<unsigned int>> getSeparation() const ;

      bool isSeparable() const ;

    protected:
      std::vector<arma::Col<unsigned int>> separation_;
      bool isSeparable_;

      double additiveSeparabilityMedianErrorThreshold_;
  };

  template <typename ParameterType, class DistanceFunction>
  MultiplicativeSeparabilityAnalysis<ParameterType, DistanceFunction>::MultiplicativeSeparabilityAnalysis()  {
    setAdditiveSeparabilityMedianErrorThreshold(0.75);
  }

  template <typename ParameterType, class DistanceFunction>
  void MultiplicativeSeparabilityAnalysis<ParameterType, DistanceFunction>::setAdditiveSeparabilityMedianErrorThreshold(
      const double& additiveSeparabilityMedianErrorThreshold) {
    if(additiveSeparabilityMedianErrorThreshold < 0 || additiveSeparabilityMedianErrorThreshold > 1) {
      throw std::runtime_error("The additive separability error threshold (" + std::to_string(additiveSeparabilityMedianErrorThreshold) + ") must be within 0 and 1.");
    }

    additiveSeparabilityMedianErrorThreshold_ = additiveSeparabilityMedianErrorThreshold;
  }
}
