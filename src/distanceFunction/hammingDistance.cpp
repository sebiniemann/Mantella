#include <mantella_bits/distanceFunction/hammingDistance.hpp>

// C++ Standard Library
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella_bits/helper/rng.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  unsigned int HammingDistance::getDistanceImplementation(
      const arma::Col<unsigned int>& firstParameter,
      const arma::Col<unsigned int>& secondParameter) const noexcept {
    return std::abs(arma::accu(secondParameter != 0) - arma::accu(firstParameter != 0));
  }

  arma::Col<unsigned int> HammingDistance::getNeighbourImplementation(
      const arma::Col<unsigned int>& parameter,
      const arma::Col<unsigned int>& minimalDistance,
      const arma::Col<unsigned int>& maximalDistance) const noexcept {
    if(minimalDistance > std::min(getDistanceImplementation(parameter), parameter.n_elem - getDistanceImplementation(parameter))) {
      throw std::logic_error("The minimal distance (" + std::to_string(minimalDistance) + ") must be lower than the absolute maximal distance (" + std::to_string(parameter.n_elem - getDistanceImplementation(parameter)) + ").");
    }

    arma::Col<unsigned int> flippedParameter = parameter;

    const arma::Col<unsigned int>& elementsToFlip = getRandomPermutation(parameter.n_elem, std::uniform_int_distribution(minimalDistance, maximalDistance));
    const arma::Col<unsigned int>& flippedElements = parameter.elem(elementsToFlip);

    flippedParameter.elem(flippedElements != 0) = 0;
    flippedParameter.elem(flippedElements == 0) = 1;
    return flippedParameter;
  }
}
