namespace mant {
  template <typename ParameterType>
  class DistanceFunction {
    public:
      explicit DistanceFunction() = default;

      ParameterType getDistance(
          const arma::Col<ParameterType>& parameter) const noexcept;

      ParameterType getDistance(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter) const;

      arma::Col<ParameterType> getNeighbour(
          const arma::Col<ParameterType>& parameter,
          const ParameterType& minimalDistance,
          const ParameterType& maximalDistance) const;

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

      virtual arma::Col<ParameterType> getNeighbourImplementation(
          const arma::Col<ParameterType>& parameter,
          const ParameterType& minimalDistance,
          const ParameterType& maximalDistance) const = 0;
  };

  template <typename ParameterType>
  ParameterType DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& parameter) const noexcept {
    return getDistanceImplementation(parameter);
  }

  template <typename ParameterType>
  ParameterType DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& firstParameter,
      const arma::Col<ParameterType>& secondParameter) const {
    if(firstParameter.n_elem != secondParameter.n_elem) {
      throw std::logic_error("The number of dimensions of the first parameter (" + std::to_string(firstParameter.n_elem) + ") must match the number of dimensions of the second parameter (" + std::to_string(secondParameter.n_elem) + ").");
    }

    return getDistance(firstParameter, secondParameter, std::is_floating_point<ParameterType>());
  }

  template <typename ParameterType>
  ParameterType DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& firstParameter,
      const arma::Col<ParameterType>& secondParameter,
      std::true_type) const noexcept {
    return getDistanceImplementation(secondParameter - firstParameter);
  }

  template <typename ParameterType>
  ParameterType DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& firstParameter,
      const arma::Col<ParameterType>& secondParameter,
      std::false_type) const noexcept {
    return getDistanceImplementation(arma::max(firstParameter, secondParameter) - arma::min(firstParameter, secondParameter));
  }

  template <typename ParameterType>
  arma::Col<ParameterType> DistanceFunction<ParameterType>::getNeighbour(
      const arma::Col<ParameterType>& parameter,
      const ParameterType& minimalDistance,
      const ParameterType& maximalDistance) const {
     if(minimalDistance < 0) {
      throw std::logic_error("The minimal distance (" + std::to_string(minimalDistance) + ") must be non-negative.");
    } else if (maximalDistance < minimalDistance) {
      throw std::logic_error("The maximal distance (" + std::to_string(maximalDistance) + ") must be greater than or equal to the minimal distance (" + std::to_string(minimalDistance) + ").");
    }

    return getNeighbourImplementation(parameter, minimalDistance, maximalDistance);
  }
}
