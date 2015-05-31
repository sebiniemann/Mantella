namespace mant {
  template <typename T, typename U = double>
  class PropertyAnalysis : public Printable {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type.");
    static_assert(std::is_floating_point<U>::value, "U must be a floating point type.");
    
    public:
      explicit PropertyAnalysis() noexcept;
      
      void setDistanceFunction(
          const std::shared_ptr<DistanceFunction<T>> distanceFunction) noexcept;

      virtual ~PropertyAnalysis() = default;

    protected:
      std::shared_ptr<DistanceFunction<T>> distanceFunction_;
  };

  //
  // Implementation
  //
  
  template <typename T, typename U>
  PropertyAnalysis<T, U>::PropertyAnalysis() noexcept {
    if (std::is_integral<T>::value) {
      setDistanceFunction(std::shared_ptr<DistanceFunction<T>>(new ManhattanDistance<T>));
    }
  }

  template <typename T, typename U>
  void PropertyAnalysis<T, U>::setDistanceFunction(
      const std::shared_ptr<DistanceFunction<T>> distanceFunction) noexcept {
    distanceFunction_ = distanceFunction;
  }
}
