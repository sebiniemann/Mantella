namespace mant {
  template <typename ParameterType>
  class PropertyAnalysis {
    public:
      explicit PropertyAnalysis() noexcept;

      void setDistanceFunction(
          std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction) noexcept;

      // Provides a default deconstructor.
      virtual ~PropertyAnalysis() = default;

    protected:
      std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction_;

      void setDefaultDistanceFunction(std::true_type) noexcept;
      void setDefaultDistanceFunction(std::false_type) noexcept;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  PropertyAnalysis<ParameterType>::PropertyAnalysis() noexcept {
    setDefaultDistanceFunction(std::is_floating_point<ParameterType>());
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
