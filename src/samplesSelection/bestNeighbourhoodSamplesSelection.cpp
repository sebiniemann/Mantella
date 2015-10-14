#include <mantella_bits/samplesSelection/bestNeighbourhoodSamplesSelection.hpp>

namespace mant {
  void BestNeighbourhoodSamplesSelection::selectImplementation() {
    arma::Col<double> bestParameter;
    double bestObjectiveValue = std::numeric_limits<double>::infinity();
    for (const auto& sample : samples_) {
      if (sample.second < bestObjectiveValue) {
        bestParameter = sample.first;
        bestObjectiveValue = sample.second;
      }
    }

    arma::Row<double> distances(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      distances(++n) = arma::norm(bestParameter - sample.first);
    }

    for (const auto& i : static_cast<arma::Row<arma::uword>>(static_cast<arma::Row<arma::uword>>(arma::sort_index(distances)).head(numberOfSelectedSamples_))) {
      const auto& selectedSample = std::next(std::begin(samples_), static_cast<decltype(samples_)::difference_type>(i));
      selectedSamples_.insert({selectedSample->first, selectedSample->second});
    }
  }

  std::string BestNeighbourhoodSamplesSelection::toString() const {
    return "best_neighbourhood_samples_selection";
  }
}
