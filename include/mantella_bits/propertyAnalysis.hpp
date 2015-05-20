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
    if(std::is_integral<T>::value) {
      setDistanceFunction(std::shared_ptr<DistanceFunction<T>>(new ManhattanDistance<T>));
    }
  }

  template <typename T>
  void PropertyAnalysis<T>::setDistanceFunction(
      const std::shared_ptr<DistanceFunction<T>> distanceFunction) noexcept {
    static_assert(std::is_integral<T>::value, "T must be an integral type.");

    distanceFunction_ = distanceFunction;
  }
}
