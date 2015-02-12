namespace mant {
  template <typename ParameterType>
  class PropertyAnalysis {
    public:
      explicit PropertyAnalysis() noexcept;

      void analyse(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      void setDistanceFunction(
          std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction) noexcept;

      double getPlausibility() const noexcept;

      // Provides a default deconstructor.
      virtual ~PropertyAnalysis() = default;

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
  PropertyAnalysis<ParameterType>::PropertyAnalysis() noexcept
    : plausibility_(0.0) {
    setDefaultDistanceFunction(std::is_floating_point<ParameterType>());
  }

  template <typename ParameterType>
  void PropertyAnalysis<ParameterType>::analyse(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept {
    plausibility_ = 0.0;

    analyseImplementation(optimisationProblem);
  }

  template <typename ParameterType>
  double PropertyAnalysis<ParameterType>::getPlausibility() const noexcept {
    return plausibility_;
  }

  template <typename ParameterType>
  void PropertyAnalysis<ParameterType>::setDistanceFunction(
      std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction) noexcept {
    distanceFunction_ = distanceFunction;
  }

  template <typename ParameterType>
  void PropertyAnalysis<ParameterType>::setDefaultDistanceFunction(
      std::true_type) noexcept {
    setDistanceFunction(std::shared_ptr<DistanceFunction<ParameterType>>(new EuclideanDistance));
  }

  template <typename ParameterType>
  void PropertyAnalysis<ParameterType>::setDefaultDistanceFunction(
      std::false_type) noexcept {
    setDistanceFunction(std::shared_ptr<DistanceFunction<ParameterType>>(new ManhattanDistance<ParameterType>));
  }
}
