#include "mantella_bits/samplesAnalysis.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>
#include <set>
#include <utility>
// IWYU pragma: no_include <ext/alloc_traits.h>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/numberTheory.hpp"
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  double fitnessDistanceCorrelation(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
    verify(samples.size() > 1, "fitnessDistanceCorrelation: The number of samples must be at least 2.");
    verify(isDimensionallyConsistent(samples), ""); // TODO

    arma::Mat<double> parameters(samples.cbegin()->first.n_elem, samples.size());
    arma::Row<double> objectiveValues(parameters.n_cols);

    arma::uword n = 0;
    for (const auto& sample : samples) {
      parameters.col(n) = sample.first;
      objectiveValues(n) = sample.second;
      ++n;
    }

    arma::uword bestParameterIndex;
    objectiveValues.min(bestParameterIndex);
    parameters.each_col() -= parameters.col(bestParameterIndex);
    parameters.shed_col(bestParameterIndex);
    objectiveValues.shed_col(bestParameterIndex);

    return arma::as_scalar(arma::cor(arma::sqrt(arma::sum(arma::square(parameters))), objectiveValues));
  }

  double lipschitzContinuity(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
    verify(samples.size() > 1, ""); // TODO
    verify(isDimensionallyConsistent(samples), ""); // TODO

    double continuity = 0.0;
    for (auto firstSample = samples.cbegin(); firstSample != samples.cend();) {
      for (auto secondSample = ++firstSample; secondSample != samples.cend(); ++secondSample) {
        continuity = std::max(continuity, std::abs(firstSample->second - secondSample->second) / arma::norm(firstSample->first - secondSample->first));
      }
    }

    return continuity;
  }

  std::vector<arma::Col<arma::uword>> additiveSeparability(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfEvaluations,
      const double maximalDeviation,
      const double minimalConfidence) {
    verify(maximalDeviation >= 0, ""); // TODO
    verify(0 < minimalConfidence && minimalConfidence <= 1, ""); // TODO
    verify(arma::all(optimisationProblem.getLowerBounds() <= optimisationProblem.getUpperBounds()), ""); // TODO

    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> partitionCandidates = twoSetsPartitions(optimisationProblem.numberOfDimensions_);

    arma::Row<arma::uword> confidences(partitionCandidates.size(), arma::fill::zeros);
    for (arma::uword n = 0; n < partitionCandidates.size(); ++n) {
      const std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>& partitionCandidate = partitionCandidates.at(n);

      const arma::Col<double>& firstSubPartLowerBounds = optimisationProblem.getLowerBounds().elem(partitionCandidate.first);
      const arma::Col<double>& firstSubPartUpperBounds = optimisationProblem.getUpperBounds().elem(partitionCandidate.first);
      const arma::Col<double>& secondSubPartLowerBounds = optimisationProblem.getLowerBounds().elem(partitionCandidate.second);
      const arma::Col<double>& secondSubPartUpperBounds = optimisationProblem.getUpperBounds().elem(partitionCandidate.second);

      for (arma::uword k = 0; k < numberOfEvaluations; ++k) {
        const arma::Col<double>& firstSubPartA = firstSubPartLowerBounds + arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem) % (firstSubPartUpperBounds - firstSubPartLowerBounds);
        const arma::Col<double>& firstSubPartB = firstSubPartLowerBounds + arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem) % (firstSubPartUpperBounds - firstSubPartLowerBounds);
        const arma::Col<double>& secondSubPartA = secondSubPartLowerBounds + arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem) % (secondSubPartUpperBounds - secondSubPartLowerBounds);
        const arma::Col<double>& secondSubPartB = secondSubPartLowerBounds + arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem) % (secondSubPartUpperBounds - secondSubPartLowerBounds);

        arma::Col<double> parameterAA(optimisationProblem.numberOfDimensions_);
        parameterAA.elem(partitionCandidate.first) = firstSubPartA;
        parameterAA.elem(partitionCandidate.second) = secondSubPartA;

        arma::Col<double> parameterBB(optimisationProblem.numberOfDimensions_);
        parameterBB.elem(partitionCandidate.first) = firstSubPartB;
        parameterBB.elem(partitionCandidate.second) = secondSubPartB;

        arma::Col<double> parameterAB(optimisationProblem.numberOfDimensions_);
        parameterAB.elem(partitionCandidate.first) = firstSubPartA;
        parameterAB.elem(partitionCandidate.second) = secondSubPartB;

        arma::Col<double> parameterBA(optimisationProblem.numberOfDimensions_);
        parameterBA.elem(partitionCandidate.first) = firstSubPartB;
        parameterBA.elem(partitionCandidate.second) = secondSubPartA;

        if (optimisationProblem.getObjectiveValue(parameterAA) + optimisationProblem.getObjectiveValue(parameterBB) - optimisationProblem.getObjectiveValue(parameterAB) - optimisationProblem.getObjectiveValue(parameterBA) <= maximalDeviation + 1e-12) {
          ++confidences(n);
        }
      }
    }

    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> acceptablePartitionCandidates;
    for (const auto acceptablePartitionCandidateIndex : static_cast<arma::Col<arma::uword>>(arma::find(confidences >= static_cast<arma::uword>(std::floor(minimalConfidence * static_cast<double>(numberOfEvaluations)))))) {
      acceptablePartitionCandidates.push_back(partitionCandidates.at(acceptablePartitionCandidateIndex));
    }

    std::vector<arma::Col<arma::uword>> partition;
    if (acceptablePartitionCandidates.size() > 1) {
      std::set<arma::uword> skipableDimensions;
      for (arma::uword n = 0; n < optimisationProblem.numberOfDimensions_; ++n) {
        if (skipableDimensions.find(n) != skipableDimensions.cend()) {
          skipableDimensions.erase(n);
        } else {
          std::vector<arma::Col<arma::uword>> contatiningPartitions;
          for (const auto& acceptablePartitionCandidate : acceptablePartitionCandidates) {
            if (!static_cast<arma::Col<arma::uword>>(arma::find(acceptablePartitionCandidate.first == n)).is_empty()) {
              contatiningPartitions.push_back(acceptablePartitionCandidate.first);
            } else {
              contatiningPartitions.push_back(acceptablePartitionCandidate.second);
            }
          }

          std::vector<arma::uword> part;
          part.push_back(n);
          for (arma::uword k = n + 1; k < optimisationProblem.numberOfDimensions_; ++k) {
            bool isWithinAllPartitions = true;
            for (const auto& contatiningPartition : contatiningPartitions) {
              if (static_cast<arma::Col<arma::uword>>(arma::find(contatiningPartition == k)).is_empty()) {
                isWithinAllPartitions = false;
                break;
              }
            }

            if (isWithinAllPartitions) {
              part.push_back(k);
              skipableDimensions.insert(k);
            }
          }

          partition.push_back(arma::Col<arma::uword>(part));
        }
      }
    } else if (acceptablePartitionCandidates.size()) {
      partition = {acceptablePartitionCandidates.at(0).first, acceptablePartitionCandidates.at(0).second};
    } else {
      partition = {range(0, optimisationProblem.numberOfDimensions_ - 1)};
    }

    return partition;
  }
}
