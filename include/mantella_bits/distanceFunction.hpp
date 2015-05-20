namespace mant {
  template <typename T>
  class DistanceFunction {
    public:
      T getLength(
          const arma::Col<T>& parameter) const noexcept;

      T getDistance(
          const arma::Col<T>& firstParameter,
          const arma::Col<T>& secondParameter) const;

      arma::Col<T> getRandomNeighbour(
          const arma::Col<T>& parameter,
          const T minimalDistance,
          const T maximalDistance) const;

      // Provides a default deconstructor.
      virtual ~DistanceFunction() = default;

    protected:
      virtual T getLengthImplementation(
          const arma::Col<T>& parameter) const noexcept = 0;

      virtual arma::Col<T> getRandomNeighbourImplementation(
          const arma::Col<T>& parameter,
          const T minimalDistance,
          const T maximalDistance) const = 0;
  };

  //
  // Implementation
  //

  template <typename T>
  T DistanceFunction<T>::getLength(
      const arma::Col<T>& parameter) const noexcept {
    return getLengthImplementation(parameter);
  }

  template <typename T>
  T DistanceFunction<T>::getDistance(
      const arma::Col<T>& firstParameter,
      const arma::Col<T>& secondParameter) const {
    verify(firstParameter.n_elem == secondParameter.n_elem, "The number of elements of the first parameter must be equal to the number of elements of the second parameter.");

    T distance;
    if(std::is_unsigned<T>::value) {
      // Avoids integer underflows / wraps for unsigned types.
      distance = getLengthImplementation(arma::max(firstParameter, secondParameter) - arma::min(firstParameter, secondParameter));
    } else {
      distance = getLengthImplementation(secondParameter - firstParameter);
    }
    
    assert(distance >= 0);

    return distance;
  }

  template <typename T>
  arma::Col<T> DistanceFunction<T>::getRandomNeighbour(
      const arma::Col<T>& parameter,
      const T minimalDistance,
      const T maximalDistance) const {
    verify(minimalDistance >= 0, "The minimal distance must be greater than or equal to 0.");
    verify(minimalDistance <= maximalDistance, "The minimal distance must be less than or equal to the maximal distance.");

    const arma::Col<T>& neighbour = getRandomNeighbourImplementation(parameter, minimalDistance, maximalDistance);

    assert(parameter.n_elem == neighbour.n_elem);
    assert(getDistance(parameter, neighbour) >= minimalDistance);
    assert(getDistance(parameter, neighbour) <= maximalDistance);

    return neighbour;
  }
}
