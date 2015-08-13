#include <mantella_bits/samplesSelection.hpp>

namespace mant {
  SamplesSelection::SamplesSelection(
      std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples)
    : samples_(samples) {
    
  }
  
  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> SamplesSelection::getSelectedSamples() const {
    return selectedSamples_;
  }
  
    
  void SamplesSelection::select() {
    selectImplementation();
  }
}