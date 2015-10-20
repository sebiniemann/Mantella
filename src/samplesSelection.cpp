#include "mantella_bits/samplesSelection.hpp"

// Mantella
#include "mantella_bits/helper/assert.hpp"

namespace mant {
  SamplesSelection::SamplesSelection(
      std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples,
      arma::uword numberOfSelectedSamples)
      : samples_(samples),
        numberOfSelectedSamples_(numberOfSelectedSamples) {
    verify(samples.size() >= numberOfSelectedSamples_, ""); // TODO
  }

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> SamplesSelection::getSelectedSamples() const {
    return selectedSamples_;
  }

  void SamplesSelection::select() {
    selectImplementation();
  }
}
