#include "mantella_bits/samplesAnalysis.hpp"

// C++ standard library
#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <utility>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/combinatorics.hpp"
#include "mantella_bits/config.hpp"
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  double fitnessDistanceCorrelation(
      const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples) {
    assert(samples.size() > 1 && "fitnessDistanceCorrelation: The number of samples must be greater than 1.");
    assert(isDimensionallyConsistent(samples) && "fitnessDistanceCorrelation: The samples must be dimensionally consistent");

    // Converts the set of samples into a matrix of parameters (each row is a dimension and each column a parameter) and a row vector of objective values, such that we can use Armadillo C++ to manipulate them.
    arma::mat parameters(samples.cbegin()->first.n_elem, samples.size());
    arma::rowvec objectiveValues(parameters.n_cols);

    arma::uword n = 0;
    for (const auto& sample : samples) {
      parameters.col(n) = sample.first;
      objectiveValues(n) = sample.second;

      ++n;
    }

    // Determines one parameter having a minimal objective value within the set of samples.
    // The best parameter is then subtracted from all other parameters, so that we can later calculated the distance towards the best one.
    arma::uword bestParameterIndex = objectiveValues.index_min();
    parameters.each_col() -= parameters.col(bestParameterIndex);

    // Excludes the best/reference parameter from the correlation, as it will always perfectly correlate with itself and therefore bias the correlation coefficient.
    parameters.shed_col(bestParameterIndex);
    objectiveValues.shed_col(bestParameterIndex);

    // Uses `arma::sum(arma::square(...))` to emulate a column-wise vector norm operator that Armadillo C++ does not have (and this is not likely to change, as it interferes with matrix norms).
    return arma::as_scalar(arma::cor(arma::sqrt(arma::sum(arma::square(parameters))), objectiveValues));
  }

  double lipschitzContinuity(
      const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples) {
    assert(samples.size() > 1 && "lipschitzContinuity: The number of samples must be greater than 1.");
    assert(isDimensionallyConsistent(samples) && "lipschitzContinuity: The samples must be dimensionally consistent");

    double lipschitzContinuity = 0.0;
    for (auto firstSample = samples.cbegin(); firstSample != samples.cend();) {
      for (auto secondSample = ++firstSample; secondSample != samples.cend(); ++secondSample) {
        lipschitzContinuity = std::max(lipschitzContinuity, std::abs(firstSample->second - secondSample->second) / arma::norm(firstSample->first - secondSample->first));
      }
    }

    return lipschitzContinuity;
  }

  std::vector<arma::uvec> additiveSeparability(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfEvaluations,
      const double minimalConfidence) {
    assert(numberOfEvaluations > 0 && "additiveSeparability: The number of evaluations must be greater than 0.");
    assert(0.0 <= minimalConfidence && minimalConfidence <= 1.0 && "additiveSeparability: The minimal confidence must be within the interval (0, 1].");

    if (!isRepresentableAsFloatingPoint(numberOfEvaluations)) {
      throw std::range_error("additiveSeparability: The number of elements must be representable as a floating point.");
    }

    if (minimalConfidence <= 0) {
      std::vector<arma::uvec> partition;
      partition.reserve(optimisationProblem.numberOfDimensions_);

      for (arma::uword n = 0; n < optimisationProblem.numberOfDimensions_; ++n) {
        partition.push_back({n});
      }

      return partition;
    }

    /* The first of two steps to analyse the additive separability of a function is to estimate all two-set separations that fulfil the deviation and confidence requirements.
     * A function *f* is additive separable into two other function *g*, *h* if the following holds:
     *
     * f(x, y) - g(x) - h(y) = 0, for all x, y
     *
     * As it is practical impossible to simply guess two separations *g*, *h* of *f*, when we only got a caller to `.getObjectiveValue(...)` and no analytic form, we use a direct consequence from the equation above instead, that must also hold true for additive separable functions and uses only *f*:
     * 
     * f(a, c) + f(b, d) - f(a, d) - f(b, c) = 0, for all a, b, c, d
     *
     */

    std::vector<std::pair<arma::uvec, arma::uvec>> partitionCandidates = twoSetsPartitions(optimisationProblem.numberOfDimensions_);

    arma::rowvec confidences(partitionCandidates.size(), arma::fill::zeros);
    for (arma::uword n = 0; n < partitionCandidates.size(); ++n) {
      const std::pair<arma::uvec, arma::uvec>& partitionCandidate = partitionCandidates.at(n);

      for (arma::uword k = 0; k < numberOfEvaluations; ++k) {
        const arma::vec& firstFirstParamter = uniformRandomNumbers(optimisationProblem.numberOfDimensions_);
        const arma::vec& secondSecondParameter = uniformRandomNumbers(optimisationProblem.numberOfDimensions_);

        arma::vec firstSecondParameter = firstFirstParamter;
        firstSecondParameter.elem(partitionCandidate.first) = secondSecondParameter.elem(partitionCandidate.first);
        arma::vec secondFirstParameter = secondSecondParameter;
        secondFirstParameter.elem(partitionCandidate.first) = firstFirstParamter.elem(partitionCandidate.first);

        // **Note:** The summation of not-a-number values results in a not-a-number value and comparing it with another value returns false, so everything will work out just fine.
        if (std::abs(optimisationProblem.getObjectiveValueOfNormalisedParameter(firstFirstParamter) + optimisationProblem.getObjectiveValueOfNormalisedParameter(secondSecondParameter) - optimisationProblem.getObjectiveValueOfNormalisedParameter(firstSecondParameter) - optimisationProblem.getObjectiveValueOfNormalisedParameter(secondFirstParameter)) < ::mant::machinePrecision) {
          ++confidences(n);
          if (confidences(n) / static_cast<double>(numberOfEvaluations) >= minimalConfidence) {
            // Proceeds with the next partition candidate, as we already reached the confidence threshold.
            break;
          }
        }
      }
    }

    const arma::uvec& acceptablePartitionCandidatesIndicies = arma::find(confidences / static_cast<double>(numberOfEvaluations) >= minimalConfidence);
    std::vector<std::pair<arma::uvec, arma::uvec>> acceptablePartitionCandidates;
    acceptablePartitionCandidates.reserve(acceptablePartitionCandidatesIndicies.n_elem);

    for (const auto acceptablePartitionCandidateIndex : acceptablePartitionCandidatesIndicies) {
      acceptablePartitionCandidates.push_back(partitionCandidates.at(acceptablePartitionCandidateIndex));
    }

    /* The last of the two steps is to calculate the partition with the maximal number of parts, from all acceptable two-set partitions.
     * If we now weaken our observation and assume that each accepted two-set partition holds true for **all** inputs, the partition with the maximal number of parts can be calculated by combining all intersections between one part and an other.
     *
     * For example, assume that we got 3 acceptable two-set partitions:
     *
     * - {{1, 2, 3, 4, 5}, {6}}
     * - {{1}, {2, 3, 4, 5, 6}}
     * - {{1, 2, 3}, {4, 5, 6}}
     *
     * We would then calculate the intersection between the first two partitions:
     *
     * {{1, 2, 3, 4, 5}, {6}} intersect {{1}, {2, 3, 4, 5, 6}} = 
     *   {{1, 2, 3, 4, 5} intersect {1}}             union
     *   {{1, 2, 3, 4, 5} intersect {2, 3, 4, 5, 6}} union
     *   {{6} intersect {1}}                         union \
     *   {{6} intersect {2, 3, 4, 5, 6}}                   / skipped and directly replaced by {{6}}
     *   = {{1}, {2, 3, 4, 5}, {6}}
     *
     * And intersect the resulting partitions with the remaining one:
     *
     * {{1}, {2, 3, 4, 5}, {6}} intersect {{1, 2, 3}, {4, 5, 6}} = 
     *   {{1} intersect {1, 2, 3}}          union \
     *   {{1} intersect {4, 5, 6}}          union / skipped and directly replaced by {{1}}
     *   {{2, 3, 4, 5} intersect {1, 2, 3}} union
     *   {{2, 3, 4, 5} intersect {4, 5, 6}} union
     *   {{6} intersect {1, 2, 3}}          union \
     *   {{6} intersect {4, 5, 6}}                / skipped and directly replaced by {{6}}
     *   = {{1}, {2, 3}, {4, 5}, {6}}
     *
     * And get {{1}, {2, 3}, {4, 5}, {6}} as partition with the maximal number of parts.
     */

    if (acceptablePartitionCandidates.size() > 1) {
      std::vector<arma::uvec> partition = {acceptablePartitionCandidates.at(0).first, acceptablePartitionCandidates.at(0).second};
      acceptablePartitionCandidates.erase(acceptablePartitionCandidates.cbegin());

      for (const auto& acceptablePartitionCandidate : acceptablePartitionCandidates) {
        std::vector<arma::uvec> nextPartition;
        for (const auto& part : partition) {
          if (part.n_elem == 1) {
            nextPartition.push_back(part);
          } else {
            // **Note:** `std::set_intersection` requires that all parts are ordered at this point.
            std::vector<arma::uword> intersection;
            std::set_intersection(part.begin(), part.end(), acceptablePartitionCandidate.first.begin(), acceptablePartitionCandidate.first.end(), intersection.begin());
            nextPartition.push_back(arma::uvec(intersection));
            intersection.clear();
            std::set_intersection(part.begin(), part.end(), acceptablePartitionCandidate.second.begin(), acceptablePartitionCandidate.second.end(), intersection.begin());
            nextPartition.push_back(arma::uvec(intersection));
          }
        }
        partition = nextPartition;

        // We are already finished, as there is no finer partition as having one part for each dimensions.
        if (partition.size() == optimisationProblem.numberOfDimensions_) {
          break;
        }
      }

      return partition;
    } else if (acceptablePartitionCandidates.size() == 1) {
      return {acceptablePartitionCandidates.at(0).first, acceptablePartitionCandidates.at(0).second};
    } else {
      return {arma::regspace<arma::uvec>(0, optimisationProblem.numberOfDimensions_ - 1)};
    }
  }

  std::vector<arma::uvec> additiveSeparability(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfEvaluations) {
    return additiveSeparability(optimisationProblem, numberOfEvaluations, 1.0);
  }
}
