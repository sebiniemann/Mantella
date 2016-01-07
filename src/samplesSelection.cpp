#include "mantella_bits/samplesSelection.hpp"

// C++ standard library
#include <iterator>
#include <utility>

// Mantella
#include "mantella_bits/probability.hpp"

namespace mant {
  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> randomly(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect) {
    verify(samples.size() >= numberOfSamplesToSelect, "randomly: The number of given samples must at least be equal to the number of samples to select.");

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> selectedSamples;
    for (const auto& i : randomPermutationVector(samples.size(), numberOfSamplesToSelect)) {
      const auto& selectedSample = std::next(std::begin(samples), static_cast<std::unordered_map<arma::Col<double>, double, Hash, IsEqual>::difference_type>(i));
      selectedSamples.insert({selectedSample->first, selectedSample->second});
    }

    return selectedSamples;
  }

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> bestFitting(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect,
      const std::function<arma::Col<double>(const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>&)> propertyAnalysis) {
    verify(static_cast<bool>(propertyAnalysis), ""); // TODO
    verify(samples.size() >= numberOfSamplesToSelect, "bestFitting: The number of given samples must at least be equal to the number of samples to select.");

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> selectedSamples;

    // TODO

    return selectedSamples;
  }

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> bestFitting(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect,
      const std::function<double(const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>&)> propertyAnalysis) {
    verify(static_cast<bool>(propertyAnalysis), ""); // TODO
    verify(samples.size() >= numberOfSamplesToSelect, "bestFitting: The number of given samples must at least be equal to the number of samples to select.");

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> selectedSamples;

    // TODO

    return selectedSamples;
  }

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> nearestNeighbours(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect,
      const arma::Col<double>& parameter,
      const std::function<double(const arma::Col<double>&, const arma::Col<double>&)> distanceFunction) {
    verify(static_cast<bool>(distanceFunction), ""); // TODO
    verify(samples.size() >= numberOfSamplesToSelect, "nearestNeighbours: The number of given samples must at least be equal to the number of samples to select.");

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> selectedSamples;

    // Generates a list of the distances towards the given parameter
    arma::Row<double> distance(samples.size());
    arma::uword n = 0;
    for (const auto& sample : samples) {
      distance(n++) = distanceFunction(parameter, sample.first);
    }

    // Sort the list of objective value and return the samples with the *numberOfSamplesToSelect* lowest ones.
    for (const auto& i : static_cast<arma::Row<arma::uword>>(static_cast<arma::Row<arma::uword>>(arma::sort_index(distance)).head(numberOfSamplesToSelect))) {
      const auto& selectedSample = std::next(std::begin(samples), static_cast<std::unordered_map<arma::Col<double>, double, Hash, IsEqual>::difference_type>(i));
      selectedSamples.insert({selectedSample->first, selectedSample->second});
    }

    return selectedSamples;
  }

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> fixedRadiusNearestNeighbours(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const double radius,
      const arma::Col<double>& parameter,
      const std::function<double(const arma::Col<double>&, const arma::Col<double>&)> distanceFunction) {
    verify(static_cast<bool>(distanceFunction), ""); // TODO

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> selectedSamples;
    for (const auto& sample : samples) {
      if (distanceFunction(parameter, sample.first) <= radius) {
        selectedSamples.insert(sample);
      }
    }

    return selectedSamples;
  }

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> elitists(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect) {
    verify(samples.size() >= numberOfSamplesToSelect, "elitists: The number of given samples must at least be equal to the number of samples to select.");

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> selectedSamples;

    // Generates a list of all objective values
    arma::Row<double> objectiveValues(samples.size());
    arma::uword n = 0;
    for (const auto& sample : samples) {
      objectiveValues(n++) = sample.second;
    }

    // Sort the list of objective value and return the samples with the *numberOfSamplesToSelect* lowest ones.
    for (const auto& i : static_cast<arma::Row<arma::uword>>(static_cast<arma::Row<arma::uword>>(arma::sort_index(objectiveValues)).head(numberOfSamplesToSelect))) {
      const auto& selectedSample = std::next(std::begin(samples), static_cast<std::unordered_map<arma::Col<double>, double, Hash, IsEqual>::difference_type>(i));
      selectedSamples.insert({selectedSample->first, selectedSample->second});
    }

    return selectedSamples;
  }
}
