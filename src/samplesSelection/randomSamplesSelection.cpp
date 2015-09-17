#include <mantella_bits/samplesSelection/randomSamplesSelection.hpp>

// Mantella
#include <mantella_bits/helper/random.hpp>

namespace mant {
  void RandomSamplesSelection::selectImplementation() {
    for (const auto& i : mant::getRandomPermutation(samples_.size(), numberOfSelectedSamples_)) {
      const auto& selectedSample = std::next(std::begin(samples_), static_cast<decltype(samples_)::difference_type>(i));
      selectedSamples_.insert({selectedSample->first, selectedSample->second});
    }
  }

  std::string RandomSamplesSelection::toString() const {
    return "random_samples_selection";
  }
}
