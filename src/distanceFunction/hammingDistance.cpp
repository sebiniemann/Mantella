#include <mantella_bits/distanceFunction/hammingDistance.hpp>

// C++ Standard Library
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella_bits/helper/rng.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  unsigned int HammingDistance::getDistanceImplementation(
      const arma::Col<unsigned int>& parameter) const noexcept {
    return arma::accu(parameter != 0);
  }

  arma::Col<unsigned int> HammingDistance::getNeighbourImplementation(
      const arma::Col<unsigned int>& parameter,
      const arma::Col<unsigned int>& minimalDistance,
      const arma::Col<unsigned int>& maximalDistance) const noexcept {
    if(arma::any(minimalDistance > 1)) {
      throw std::logic_error("The minimal distance (" + std::to_string(minimalDistance) + ") must be lower than or equal to 1 for each element.");
    }

    // TODO Fix me

    arma::Col<unsigned int> flippedParameter = parameter;

    const arma::Col<unsigned int>& elementsToFlip = getRandomPermutation(parameter.n_elem, std::uniform_int_distribution(arma::accu(minimalDistance != 0), arma::accu(maximalDistance != 0)));
    const arma::Col<unsigned int>& flippedElements = parameter.elem(elementsToFlip);

    flippedParameter.elem(flippedElements != 0) = 0;
    flippedParameter.elem(flippedElements == 0) = 1;
    return flippedParameter;
  }
}
