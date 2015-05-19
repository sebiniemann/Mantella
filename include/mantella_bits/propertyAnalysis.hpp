namespace mant {
  template <typename T>
  class PropertyAnalysis {
    public:
      explicit PropertyAnalysis() noexcept;

      void setDistanceFunction(
          const std::shared_ptr<DistanceFunction<T>> distanceFunction) noexcept;

      // Provides a default deconstructor.
      virtual ~PropertyAnalysis() = default;

    protected:
      std::shared_ptr<DistanceFunction<T>> distanceFunction_;

      void setDefaultDistanceFunction(std::true_type) noexcept;
      void setDefaultDistanceFunction(std::false_type) noexcept;
  };

  //
  // Implementation
  //

  template <typename T>
  PropertyAnalysis<T>::PropertyAnalysis() noexcept {
    setDefaultDistanceFunction(std::is_floating_point<T>());
  }

  template <typename T>
  void PropertyAnalysis<T>::setDistanceFunction(
      const std::shared_ptr<DistanceFunction<T>> distanceFunction) noexcept {
    distanceFunction_ = distanceFunction;
  }

  template <typename T>
  void PropertyAnalysis<T>::setDefaultDistanceFunction(
      std::true_type) noexcept {
    setDistanceFunction(std::shared_ptr<DistanceFunction<T>>(new EuclideanDistance));
  }

  template <typename T>
  void PropertyAnalysis<T>::setDefaultDistanceFunction(
      std::false_type) noexcept {
    setDistanceFunction(std::shared_ptr<DistanceFunction<T>>(new ManhattanDistance<T>));
  }
}
