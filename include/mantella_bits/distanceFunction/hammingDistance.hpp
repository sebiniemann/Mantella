namespace mant {
  template <typename T>
  class HammingDistance : public DistanceFunction<T> {
    public:
      explicit HammingDistance(
          const T lowerBound,
          const T upperBound);

    protected:
      const T lowerBound_;
      const T upperBound_;
    
      T getLengthImplementation(
          const arma::Col<T>& parameter) const noexcept override;

      arma::Col<T> getRandomNeighbourImplementation(
          arma::Col<T> parameter,
          const T minimalDistance,
          const T maximalDistance) const override;
  };

  //
  // Implementation
  //

  template <typename T>
  HammingDistance<T>::HammingDistance(
      const T lowerBound,
      const T upperBound)
    : lowerBound_(lowerBound),
      upperBound_(upperBound) {
    verify(lowerBound_ <= upperBound_, "The lower bound must be less than or equal to the upper bound.");
  }

  template <typename T>
  T HammingDistance<T>::getLengthImplementation(
      const arma::Col<T>& parameter) const noexcept {
    return arma::accu(parameter != 0);
  }

  template <typename T>
  arma::Col<T> HammingDistance<T>::getRandomNeighbourImplementation(
      arma::Col<T> parameter,
      const T minimalDistance,
      const T maximalDistance) const {
    assert(minimalDistance >= 0);
    assert(minimalDistance <= maximalDistance);

    // The maximal distance of the Hamming distance is limited by the number of elements.
    verify(maximalDistance <= parameter.n_elem, "The maximal distance must be less than or equal to the number of elements.");

    // Chooses the number and indices of the elements to be changed.
    for(const T element : getRandomPermutation(parameter.n_elem, std::uniform_int_distribution<T>(minimalDistance, maximalDistance)(Rng::getGenerator()))) {
      // Selects a new value from the range [min, max - 1] ...
      T newValue = std::uniform_int_distribution<T>(lowerBound_, upperBound_ - 1)(Rng::getGenerator());
      
      // ... and adds +1, if it was within [current, max - 1], setting the actual range to
      // [min, current - 1], [current + 1, max].
      if(parameter(element) >= newValue) {
        newValue += 1;
      }
      
      parameter(element) = newValue;
    }
    
    return parameter;
  }
}
