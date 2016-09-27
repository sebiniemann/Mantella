#pragma once

/**

*/
std::vector<> twoSetsPartitions(
    const arma::uword numberOfElements) {
  assert(numberOfElements > 1 && "twoSetsPartitions: The number of elements must be greater than 1.");

  if (numberOfElements == 2) {
    // Added `std::initializer_list<...>` to resolve constructor ambiguity.
    return {std::initializer_list<arma::uvec>({{0}}), {{1}}};
  }

  /* Generates all partitions with two sets by permuting a bit mask of the same length as `numberOfElements`, having *k* 1's for each index being part of the first part and 0's for the second one. *k* denotes the amount of elements that is distributed to each part and iterated up to `numberOfElements` / 2, as we would add duplicates (since there is no order between being the first or second part of a partition) of previously added permutations for greater values.
   * If `numberOfElements` was set to 4, we would get:
   * 
   * k = 1:
   *   (1, 0, 0, 0) Bit mask (initial)
   *    |
   *   (0         ) First part  \
   *       |  |  |               |- First partition
   *   (   1  2  3) Second part /
   *
   *
   *   (1, 0, 0, 0) Bit mask (permuted in reverse lexicographic)
   *       |
   *   (   1      ) First part  \
   *    |     |  |               |- Second partition
   *   (0     2  3) Second part /
   *
   *   ...
   * 
   * k = 2:
   *   (1, 1, 0, 0) Bit mask (initial)
   *    |  |
   *   (0, 1      ) First part  \
   *          |  |               |- Fifth partition
   *   (      2  3) Second part /
   *
   *
   *   (1, 0, 1, 0) Bit mask (permuted in reverse lexicographic)
   *    |     |
   *   (0,    2   ) First part  \
   *       |     |               |- Sixth partition
   *   (   1     3) Second part /
   *
   *   ...
   */

  // Checks that the later `static_cast<decltype(bitmask)::difference_type>(k)` cast is safe for all given `k`.
  if (numberOfElements / 2 > std::numeric_limits<std::vector<arma::uword>::difference_type>::max()) {
    throw std::range_error("twoSetsPartitions: The halved number of elements must be less than or equal to the largest representable iterator difference.");
  }

  arma::field<arma::uvec> partitions(2, secondStirlingNumber(numberOfElements, 2));
  const arma::uvec& elements = arma::regspace<arma::uvec>(0, numberOfElements - 1);
  // **Note:** The implicit integer flooring is the wanted behaviour.
  for (arma::uword n = 0, k = 1; k <= numberOfElements / 2; ++k) {
    // **Note:** The C++ standard guarantees that the vector initialises all its elements to 0, wherefore we are only setting the first `k` elements to 1.
    std::vector<arma::uword> bitmask(numberOfElements);
    std::fill(bitmask.begin(), std::next(bitmask.begin(), static_cast<decltype(bitmask)::difference_type>(k)), 1);
    do {
      /* Avoids adding duplicates when we split the elements in equally sized
       * parts, skipping the second half of permutations in this case.
       * If `numberOfElements` was set to 4, we would get:
       *
       * k = 2
       *   (1, 1, 0, 0) (Bit mask)
       *   (1, 0, 1, 0) (Bit mask)
       *   (1, 0, 0, 1) (Bit mask)
       *   (0, 1, 1, 0) (Bit mask) Invert of the third bit mask <- Skipped from here on
       *   (0, 1, 0, 1) (Bit mask) Invert of the second bit mask
       *   (0, 0, 1, 1) (Bit mask) Invert of the first bit mask
       */
      if (2 * k == numberOfElements && bitmask.at(0) != 1) {
        break;
      }

      // Uses `bitmask`'s memory directly without copying it.
      partitions(1, n) = elements.elem(arma::find(arma::uvec(&bitmask[0], numberOfElements, false) == 1));
      partitions(2, n) = elements.elem(arma::find(arma::uvec(&bitmask[0], numberOfElements, false) == 0));
      ++n;
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
  }

  return partitions;
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