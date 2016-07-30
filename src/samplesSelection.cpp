#include "mantella_bits/samplesSelection.hpp"

// C++ standard library
#include <cassert>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  std::unordered_map<arma::vec, double, Hash, IsEqual> elitists(
      const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect) {
    assert(samples.size() >= numberOfSamplesToSelect && "elitists: The number of given samples must be greater than or equal to the number of samples to select.");
    assert(isDimensionallyConsistent(samples) && "elitists: The samples must be dimensionally consistent.");

    if (samples.size() == 0) {
      return {};
    }

    if (samples.size() - 1 > std::numeric_limits<std::unordered_map<arma::vec, double, Hash, IsEqual>::difference_type>::max()) {
      throw std::range_error("elitists: The number of samples must be less than or equal to the largest representable iterator difference type.");
    }

    // Generates a list of all objective values
    arma::rowvec objectiveValues(samples.size());
    arma::uword n = 0;
    for (const auto& sample : samples) {
      objectiveValues(n++) = sample.second;
    }

    // Sorts the list of objective value and returns the samples with the ´numberOfSamplesToSelect` lowest objective values.
    std::unordered_map<arma::vec, double, Hash, IsEqual> selectedSamples;
    for (const auto& i : arma::uvec(arma::uvec(arma::sort_index(objectiveValues)).head(numberOfSamplesToSelect))) {
      // **Note:** C++ guarantees under C++11 23.2.5/8 and C++11 23.2.5/14 that the iteration order does not change as long as the container is not resized. This is very important, as we use the iteration order of the range-based for loop, that filled `objectiveValues`, to select the sample.
      const auto& selectedSample = std::next(std::begin(samples), static_cast<decltype(selectedSamples)::difference_type>(i));
      selectedSamples.insert({selectedSample->first, selectedSample->second});
    }

    return selectedSamples;
  }
}
