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
      arma::mat parameters,
      arma::rowvec objectiveValues) {
    assert(parameters.n_cols > 1 && "fitnessDistanceCorrelation: The number of parameters must be greater than 1.");
    assert(parameters.n_cols == objectiveValues.n_elem && "fitnessDistanceCorrelation: The number of parameters must be equal to the number of objective values.");

    // Determines one parameter having a minimal objective value within the set of samples.
    // The best parameter is then subtracted from all other parameters, so that we can later calculated the distance towards the best one.
    const arma::uword bestParameterIndex = objectiveValues.index_min();
    parameters.each_col() -= parameters.col(bestParameterIndex);

    // Excludes the best/reference parameter from the correlation, as it will always perfectly correlate with itself and therefore bias the correlation coefficient.
    parameters.shed_col(bestParameterIndex);
    objectiveValues.shed_col(bestParameterIndex);

    // Uses `arma::sum(arma::square(...))` to emulate a column-wise vector norm operator that Armadillo C++ does not have (and this is not likely to change, as it interferes with matrix norms).
    return arma::as_scalar(arma::cor(arma::sqrt(arma::sum(arma::square(parameters))), objectiveValues));
  }

  double lipschitzContinuity(
      const arma::mat& parameters,
      const arma::rowvec& objectiveValues) {
    assert(parameters.n_cols > 1 && "fitnessDistanceCorrelation: The number of parameters must be greater than 1.");
    assert(parameters.n_cols == objectiveValues.n_elem && "fitnessDistanceCorrelation: The number of parameters must be equal to the number of objective values.");

    double lipschitzContinuity = 0.0;
    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      const arma::vec& firstParameter = parameters.col(n);
      const double firstObjectiveValue = objectiveValues(n);
      for (arma::uword k = n++; k < parameters.n_cols; ++k) {
        lipschitzContinuity = std::max(lipschitzContinuity, std::abs(objectiveValues(k) - firstObjectiveValue) / arma::norm(parameters.col(k) - firstParameter));
      }
    }

    return lipschitzContinuity;
  }

  arma::field<arma::uvec> additiveSeparability(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfEvaluations,
      const double minimalConfidence) {
    assert(numberOfEvaluations > 0 && "additiveSeparability: The number of evaluations must be greater than 0.");
    assert(0.0 <= minimalConfidence && minimalConfidence <= 1.0 && "additiveSeparability: The minimal confidence must be within the interval (0, 1].");

    if (!isRepresentableAsFloatingPoint(numberOfEvaluations)) {
      throw std::range_error("additiveSeparability: The number of elements must be representable as a floating point.");
    }

    if (minimalConfidence <= 0) {
      arma::field<arma::uvec> partition(1, numberOfEvaluations);
      for (arma::uword n = 0; n < partition.n_elem; ++n) {
        partition(n) = {n};
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

    const arma::field<arma::uvec>& partitionCandidates = twoSetsPartitions(optimisationProblem.numberOfDimensions_);
    arma::rowvec confidences(partitionCandidates.n_cols, arma::fill::zeros);
    for (arma::uword n = 0; n < partitionCandidates.n_cols; ++n) {
      const arma::uvec& firstPart = partitionCandidates(0, n);
      const arma::uvec& secondPart = partitionCandidates(1, n);
      
      for (arma::uword k = 0; k < numberOfEvaluations; ++k) {
        const arma::vec& parameterAC = uniformRandomNumbers(optimisationProblem.numberOfDimensions_);
        const arma::vec& parameterBD = uniformRandomNumbers(optimisationProblem.numberOfDimensions_);
        
        // **Note:** The summation of not-a-number values results in a not-a-number value and comparing it with another value returns false, so everything will work out just fine.
        if (std::abs(optimisationProblem.getObjectiveValueOfNormalisedParameter(parameterAC) + optimisationProblem.getObjectiveValueOfNormalisedParameter(parameterBD) - optimisationProblem.getObjectiveValueOfNormalisedParameter(arma::join_cols(parameterAC.elem(firstPart), parameterBD.elem(secondPart))) - optimisationProblem.getObjectiveValueOfNormalisedParameter(arma::join_cols(parameterBD.elem(firstPart), parameterAC.elem(secondPart)))) < ::mant::machinePrecision) {
          confidences(n) += 1.0 / static_cast<double>(numberOfEvaluations);
          if (confidences(n) >= minimalConfidence) {
            // Proceeds with the next partition candidate, as we already reached the confidence threshold.
            break;
          }
        }
      }
    }

    const arma::uvec& acceptablePartitionsIndices = arma::find(confidences >= minimalConfidence);
    arma::field<arma::uvec> acceptablePartitions(partitionCandidates.n_rows, acceptablePartitionsIndices.n_elem);
    for (arma::uword n = 0; n < acceptablePartitionsIndices.n_elem; ++n) {
      acceptablePartitions.col(n) = partitionCandidates.col(n);
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

    if (acceptablePartitions.n_cols > 1) {
      std::vector<arma::uvec> partition({acceptablePartitions(0, 0), acceptablePartitions(1, 0)});
      for (arma::uword n = 1; n < acceptablePartitions.n_cols; ++n) {
        const arma::field<arma::uvec>& acceptablePartition = acceptablePartitions.col(n);
        
        std::vector<arma::uvec> nextPartition;
        nextPartition.reserve(2 * partition.size());
        for (const auto& part : partition) {
          if (part.n_elem == 1) {
            nextPartition.push_back(part);
          } else {
            // **Note:** `std::set_intersection` requires that all parts are ordered at this point.
            std::vector<arma::uword> intersection;
            std::set_intersection(part.begin(), part.end(), acceptablePartition(0).begin(), acceptablePartition(0).end(), intersection.begin());
            if (intersection.size() > 0) {
              nextPartition.push_back(arma::uvec(&intersection[0], intersection.size(), false));
            }
            
            intersection.clear();
            std::set_intersection(part.begin(), part.end(), acceptablePartition(1).begin(), acceptablePartition(1).end(), intersection.begin());
            if (intersection.size() > 0) {
              nextPartition.push_back(arma::uvec(&intersection[0], intersection.size(), false));
            }
          }
        }
        partition = nextPartition;

        // We are already finished, as there is no finer partition as having one part for each dimensions.
        if (partition.size() == optimisationProblem.numberOfDimensions_) {
          break;
        }
      }

      arma::field<arma::uvec> separation(partition.size(), 1);
      for (arma::uword n = 0; n < partition.size(); ++n) {
        separation(n) = partition.at(n);
      }
      
      return separation;
    } else if (acceptablePartitions.n_cols == 1) {
      return {acceptablePartitions(0), acceptablePartitions(1)};
    } else {
      return {arma::regspace<arma::uvec>(0, optimisationProblem.numberOfDimensions_ - 1)};
    }
  }

  arma::field<arma::uvec> additiveSeparability(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfEvaluations) {
    return additiveSeparability(optimisationProblem, numberOfEvaluations, 1.0);
  }
}
