namespace mant {
  template <typename ParameterType>
  class ManhattanDistance : public DistanceFunction<ParameterType> {
    protected:
      ParameterType getDistanceImplementation(
          const arma::Col<ParameterType>& parameter) const noexcept override;

      arma::Col<ParameterType> getRandomNeighbourImplementation(
          const arma::Col<ParameterType>& parameter,
          const ParameterType& minimalDistance,
          const ParameterType& maximalDistance) const noexcept override;

      arma::Col<ParameterType> getRandomNeighbourImplementation(
          const arma::Col<ParameterType>& parameter,
          const ParameterType& minimalDistance,
          const ParameterType& maximalDistance,
          std::true_type) const noexcept;

      arma::Col<ParameterType> getRandomNeighbourImplementation(
          const arma::Col<ParameterType>& parameter,
          const ParameterType& minimalDistance,
          const ParameterType& maximalDistance,
          std::false_type) const noexcept;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  ParameterType ManhattanDistance<ParameterType>::getDistanceImplementation(
      const arma::Col<ParameterType>& parameter) const noexcept {
    return arma::norm(parameter, 1);
  }

  // TODO Minus / int / unsigned int
  template <typename ParameterType>
  arma::Col<ParameterType> ManhattanDistance<ParameterType>::getRandomNeighbourImplementation(
      const arma::Col<ParameterType>& parameter,
      const ParameterType& minimalDistance,
      const ParameterType& maximalDistance) const noexcept {
    return getRandomNeighbourImplementation(parameter, minimalDistance, maximalDistance, std::is_floating_point<ParameterType>());
  }

  template <typename ParameterType>
  arma::Col<ParameterType> ManhattanDistance<ParameterType>::getRandomNeighbourImplementation(
      const arma::Col<ParameterType>& parameter,
      const ParameterType& minimalDistance,
      const ParameterType& maximalDistance,
      std::true_type) const noexcept {
    assert(minimalDistance <= maximalDistance);

    return arma::normalise(2.0 * arma::randu<arma::Col<double>>(parameter.n_elem) - 1.0, 1) * std::uniform_real_distribution<ParameterType>(minimalDistance, maximalDistance)(Rng::getGenerator());
  }

  template <typename ParameterType>
  arma::Col<ParameterType> ManhattanDistance<ParameterType>::getRandomNeighbourImplementation(
      const arma::Col<ParameterType>& parameter,
      const ParameterType& minimalDistance,
      const ParameterType& maximalDistance,
      std::false_type) const noexcept {
    assert(minimalDistance <= maximalDistance);

  }
}
