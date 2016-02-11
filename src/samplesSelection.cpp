#include "mantella_bits/samplesSelection.hpp"

// C++ standard library
#include <iterator>
#include <utility>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> elitists(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect) {
    verify(samples.size() >= numberOfSamplesToSelect, "elitists: The number of (provided) samples must be greater than or equal to the number of samples to select.");
    verify(isDimensionallyConsistent(samples), "elitists: The samples must be dimensionally consistent.");

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> selectedSamples;

    // Generates a list of all objective values
    arma::Row<double> objectiveValues(samples.size());
    arma::uword n = 0;
    for (const auto& sample : samples) {
      objectiveValues(n++) = sample.second;
    }

    // Sorts the list of objective value and returns the samples with the *numberOfSamplesToSelect* lowest objective values.
    for (const auto& i : static_cast<arma::Col<arma::uword>>(static_cast<arma::Col<arma::uword>>(arma::sort_index(objectiveValues)).head(numberOfSamplesToSelect))) {
      const auto& selectedSample = std::next(std::begin(samples), static_cast<std::unordered_map<arma::Col<double>, double, Hash, IsEqual>::difference_type>(i));
      selectedSamples.insert({selectedSample->first, selectedSample->second});
    }

    return selectedSamples;
  }
}
