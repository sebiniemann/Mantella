namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class LipschitzContinuityAnalysis : public ContinuityAnalysis<ParameterType, DistanceFunction> {
    public:
      explicit LipschitzContinuityAnalysis() ;

      LipschitzContinuityAnalysis(const LipschitzContinuityAnalysis&) = delete;
      LipschitzContinuityAnalysis& operator=(const LipschitzContinuityAnalysis&) = delete;

      double getLipschitzConstant() const ;

    protected:
      double lipschitzConstant_;
  };

  template <typename ParameterType, class DistanceFunction>
  LipschitzContinuityAnalysis<ParameterType, DistanceFunction>::LipschitzContinuityAnalysis() 
    : lipschitzConstant_(0.0) {

  }

  template <typename ParameterType, class DistanceFunction>
  double LipschitzContinuityAnalysis<ParameterType, DistanceFunction>::getLipschitzConstant() const  {
    return lipschitzConstant_;
  }
}
