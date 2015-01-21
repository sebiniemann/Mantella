#pragma once

// Mantella
#include <mantella_bits/distanceFunction.hpp>
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  template <typename ParameterType>
  class ManhattanDistance : public DistanceFunction<ParameterType> {
    protected:
      ParameterType getDistanceImplementation(
          const arma::Col<ParameterType>& parameter) const  override;

      arma::Col<ParameterType> getNeighbourImplementation(
          const arma::Col<ParameterType>& parameter,
          const ParameterType& minimalDistance,
          const ParameterType& maximalDistance) const  override;

      arma::Col<ParameterType> getNeighbourImplementation(
          const arma::Col<ParameterType>& parameter,
          const ParameterType& minimalDistance,
          const ParameterType& maximalDistance,
          std::true_type) const ;

      arma::Col<ParameterType> getNeighbourImplementation(
          const arma::Col<ParameterType>& parameter,
          const ParameterType& minimalDistance,
          const ParameterType& maximalDistance,
          std::false_type) const ;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  ParameterType ManhattanDistance<ParameterType>::getDistanceImplementation(
      const arma::Col<ParameterType>& parameter) const  {
    return arma::norm(parameter, 1);
  }

  // TODO Minus / int / unsigned int
  template <typename ParameterType>
  arma::Col<ParameterType> ManhattanDistance<ParameterType>::getNeighbourImplementation(
      const arma::Col<ParameterType>& parameter,
      const ParameterType& minimalDistance,
      const ParameterType& maximalDistance) const  {
    return getNeighbourImplementation(parameter, minimalDistance, maximalDistance, std::is_floating_point<ParameterType>());
  }

  template <typename ParameterType>
  arma::Col<ParameterType> ManhattanDistance<ParameterType>::getNeighbourImplementation(
      const arma::Col<ParameterType>& parameter,
      const ParameterType& minimalDistance,
      const ParameterType& maximalDistance,
      std::true_type) const  {
    return arma::normalise(2.0 * arma::randu(parameter.n_elem) - 1.0, 1) * std::uniform_real_distribution<ParameterType>(minimalDistance, maximalDistance)(Rng::getGenerator());
  }

  template <typename ParameterType>
  arma::Col<ParameterType> ManhattanDistance<ParameterType>::getNeighbourImplementation(
      const arma::Col<ParameterType>& parameter,
      const ParameterType& minimalDistance,
      const ParameterType& maximalDistance,
      std::false_type) const  {

  }
}
