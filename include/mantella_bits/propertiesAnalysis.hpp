namespace mant {
  template <typename ParameterType>
  class PropertiesAnalysis {
    public:
      explicit PropertiesAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesAnalysis(const PropertiesAnalysis&) = delete;
      PropertiesAnalysis& operator=(const PropertiesAnalysis&) = delete;

      void analyse(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      void setDistanceFunction(
          std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction) noexcept;

      double getPlausibility() const noexcept;

    protected:
      double plausibility_;

      std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction_;

      void setDefaultDistanceFunction(std::true_type) noexcept;
      void setDefaultDistanceFunction(std::false_type) noexcept;

      virtual void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  PropertiesAnalysis<ParameterType>::PropertiesAnalysis() noexcept
    : plausibility_(0.0) {
    setDefaultDistanceFunction(std::is_floating_point<ParameterType>());
  }

  template <typename ParameterType>
  void PropertiesAnalysis<ParameterType>::analyse(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept {
    plausibility_ = 0.0;

    analyseImplementation(optimisationProblem);
  }

  template <typename ParameterType>
  double PropertiesAnalysis<ParameterType>::getPlausibility() const noexcept {
    return plausibility_;
  }

  template <typename ParameterType>
  void PropertiesAnalysis<ParameterType>::setDistanceFunction(
      std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction) noexcept {
    distanceFunction_ = distanceFunction;
  }

  template <typename ParameterType>
  void PropertiesAnalysis<ParameterType>::setDefaultDistanceFunction(
      std::true_type) noexcept {
    setDistanceFunction(new EuclideanDistance);
  }

  template <typename ParameterType>
  void PropertiesAnalysis<ParameterType>::setDefaultDistanceFunction(
      std::false_type) noexcept {
    setDistanceFunction(new ManhattanDistance<ParameterType>);
  }
}
