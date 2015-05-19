namespace mant {
  template <typename T>
  class ManhattanDistance : public DistanceFunction<T> {
    public:
      using DistanceFunction<T>::DistanceFunction;

    protected:
      T getLengthImplementation(
          const arma::Col<T>& parameter) const noexcept override;

      arma::Col<T> getRandomNeighbourImplementation(
          const arma::Col<T>& parameter,
          const T minimalDistance,
          const T maximalDistance) const noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  T ManhattanDistance<T>::getLengthImplementation(
      const arma::Col<T>& parameter) const noexcept {
    return arma::norm(parameter, 1);
  }

  template <typename T>
  arma::Col<T> ManhattanDistance<T>::getRandomNeighbourImplementation(
      const arma::Col<T>& parameter,
      const T minimalDistance,
      const T maximalDistance) const noexcept {
    assert(minimalDistance >= 0);
    assert(minimalDistance <= maximalDistance);

    return parameter +
      // Discretises the value by rounding towards the nearest integer value.
      arma::round(
        // Chooses randomly and uniformly a direction and length for each element, normalising their
        // sum to 1.
        arma::normalise(arma::randu<arma::Col<long double>>(parameter.n_elem) - 0.5, 1) *
        // Chooses randomly and uniformly the length of the vector from the open interval 
        // (min - 0.5, max + 0.5).
        // *Note:* The interval must be open at both sides, to support positive and negative
        // directions.
        std::uniform_real_distribution<T>(
          std::nexttoward(minimalDistance - 0.5, maximalDistance),
          std::nexttoward(maximalDistance + 0.5, minimalDistance)
        )(Rng::getGenerator())
      );
  }
}