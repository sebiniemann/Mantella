#include <mantella_bits/samplesSelection/bestSamplesSelection.hpp>

namespace mant {
  void BestSamplesSelection::selectImplementation() {
    arma::Col<double> objectiveValues(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      objectiveValues(++n) = sample.second;
    }

    for (const auto& i : static_cast<arma::Col<arma::uword>>(static_cast<arma::Col<arma::uword>>(arma::sort_index(objectiveValues)).head(numberOfSelectedSamples_))) {
      const auto& selectedSample = std::next(std::begin(samples_), static_cast<decltype(samples_)::difference_type>(i));
      selectedSamples_.insert({selectedSample->first, selectedSample->second});
    }
  }
  
  std::string BestSamplesSelection::toString() const {
    return "best_samples_selection";
  }
}
