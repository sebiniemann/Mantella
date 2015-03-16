namespace mant {
  template <typename ParameterType>
  class DistanceFunction {
    public:
      ParameterType getDistance(
          const arma::Col<ParameterType>& parameter) const noexcept;

      ParameterType getDistance(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter) const;

      arma::Col<ParameterType> getRandomNeighbour(
          const arma::Col<ParameterType>& parameter,
          const ParameterType minimalDistance,
          const ParameterType maximalDistance) const;

      // Provides a default deconstructor.
      virtual ~DistanceFunction() = default;

    protected:
      virtual ParameterType getDistanceImplementation(
          const arma::Col<ParameterType>& parameter) const noexcept = 0;

      ParameterType getDistance(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter,
          std::true_type) const noexcept;

      ParameterType getDistance(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter,
          std::false_type) const noexcept;

      virtual arma::Col<ParameterType> getRandomNeighbourImplementation(
          const arma::Col<ParameterType>& parameter,
          const ParameterType minimalDistance,
          const ParameterType maximalDistance) const = 0;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  ParameterType DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& parameter) const noexcept {
    return getDistanceImplementation(parameter);
  }

  template <typename ParameterType>
  ParameterType DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& firstParameter,
      const arma::Col<ParameterType>& secondParameter) const {
    checkCompatibleDimension("The number of elements of the first parameter", firstParameter.n_elem, "the number of elements of the second", secondParameter.n_elem);

    const double& distance = getDistance(firstParameter, secondParameter, std::is_floating_point<ParameterType>());

    assert(distance >= 0);

    return distance;
  }

  template <typename ParameterType>
  ParameterType DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& firstParameter,
      const arma::Col<ParameterType>& secondParameter,
      std::true_type) const noexcept {
    assert(isDimensionCompatible(firstParameter.n_elem, secondParameter.n_elem));

    return getDistanceImplementation(secondParameter - firstParameter);
  }

  template <typename ParameterType>
  ParameterType DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& firstParameter,
      const arma::Col<ParameterType>& secondParameter,
      std::false_type) const noexcept {
    assert(isDimensionCompatible(firstParameter.n_elem, secondParameter.n_elem));

    return getDistanceImplementation(arma::max(firstParameter, secondParameter) - arma::min(firstParameter, secondParameter));
  }

  template <typename ParameterType>
  arma::Col<ParameterType> DistanceFunction<ParameterType>::getRandomNeighbour(
      const arma::Col<ParameterType>& parameter,
      const ParameterType minimalDistance,
      const ParameterType maximalDistance) const {
    if(minimalDistance < 0) {
      throw std::logic_error("The minimal distance (" + std::to_string(minimalDistance) + ") must be non-negative.");
    } else if (maximalDistance < minimalDistance) {
      throw std::logic_error("The maximal distance (" + std::to_string(maximalDistance) + ") must be greater than or equal to the minimal distance (" + std::to_string(minimalDistance) + ").");
    }

   const arma::Col<ParameterType>& neighbour = getRandomNeighbourImplementation(parameter, minimalDistance, maximalDistance);

   assert(parameter.n_elem == neighbour.n_elem);
   assert(getDistance(parameter, neighbour) >= minimalDistance);
   assert(getDistance(parameter, neighbour) <= maximalDistance);

    return neighbour;
  }
}
