#include "mantella_bits/numericalAnalysis.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  // To understand the following equations, we advise to carefully read the following paper:
  // @see R. P. Brent (1971). An algorithm with guaranteed convergence for finding a zero of a function. The Computer Journal, 14(4), pp. 422-425.
  double brent(
      std::function<double(double)> objectiveFunction,
      double lowerBound,
      double upperBound,
      arma::uword maximalNumberOfIterations,
      double acceptableTolerance) {
    verify(static_cast<bool>(objectiveFunction), "brent: The objective function must be callable.");
    verify(acceptableTolerance >= 0, "brent: The acceptable tolerance must be positive.");
    verify(lowerBound <= upperBound, "brent: The lower bound must be less than or equal to the upper bound.");

    double lowerBoundObjectiveValue = objectiveFunction(lowerBound);

    // Returns the lower bound, if both bounds are equal and a root.
    if (std::abs(lowerBound - upperBound) < 1e-12) {
      if (lowerBoundObjectiveValue <= acceptableTolerance) {
        return lowerBound;
      } else {
        return arma::datum::nan;
      }
    }

    // Returns the lower bound, if its already close enough to the root.
    if (std::abs(lowerBoundObjectiveValue) <= acceptableTolerance) {
      return lowerBound;
    }

    double upperBoundObjectiveValue = objectiveFunction(upperBound);

    // Returns the upper bound, if its already close enough to the root.
    if (std::abs(upperBoundObjectiveValue) <= acceptableTolerance) {
      return upperBound;
    }

    verify(lowerBoundObjectiveValue * upperBoundObjectiveValue <= 0.0 || !std::isfinite(lowerBoundObjectiveValue) || !std::isfinite(upperBoundObjectiveValue), "brent: The lower bound and upper bound objective value must have a different sign.");

    // The previous upper bound is used within the interpolation and **must** be different from the current upper bound.
    // Therefore, the initial, previous upper bound is set to the lower bound.
    double previousUpperBound = lowerBound;
    double previousUpperBoundObjectiveValue = lowerBoundObjectiveValue;

    for (arma::uword numberOfIterations = 0; numberOfIterations < maximalNumberOfIterations && std::abs(upperBoundObjectiveValue) > acceptableTolerance; ++numberOfIterations) {
      // In Brent's algorithm, the step is always taken from the upper bound and assumed to be closest to the root (assuming a linear approximation).
      // Therefore, if the lower bound is closer, we swap their values.
      if (std::abs(lowerBoundObjectiveValue) < std::abs(upperBoundObjectiveValue)) {
        previousUpperBound = upperBound;
        previousUpperBoundObjectiveValue = upperBoundObjectiveValue;

        std::swap(upperBound, lowerBound);
        std::swap(upperBoundObjectiveValue, lowerBoundObjectiveValue);
      }

      double middleOfBounds = (lowerBound - upperBound) / 2.0;

      double stepSize;
      // In case the correlation "a lower value is closer to the root" is incorrect, the interpolation is skipped and a bisection is performed.
      if (std::abs(previousUpperBoundObjectiveValue) <= std::abs(upperBoundObjectiveValue) || !std::isfinite(lowerBoundObjectiveValue) || !std::isfinite(upperBoundObjectiveValue)) {
        stepSize = middleOfBounds;
      } else {
        double upperToPreviousUpperBoundObjectiveValueRatio = upperBoundObjectiveValue / previousUpperBoundObjectiveValue;
        // Uses an inverse quadratic interpolation if we got 3 different points and otherwise a linear instead.
        if (std::abs(previousUpperBound - lowerBound) > 0.0) {
          double previousUpperToLowerBoundObjectiveValueRatio = previousUpperBoundObjectiveValue / lowerBoundObjectiveValue;
          double upperToLowerBoundObjectiveValueRatio = upperBoundObjectiveValue / lowerBoundObjectiveValue;
          stepSize = upperToPreviousUpperBoundObjectiveValueRatio * (2.0 * middleOfBounds * previousUpperToLowerBoundObjectiveValueRatio * (previousUpperToLowerBoundObjectiveValueRatio - upperToLowerBoundObjectiveValueRatio) - (upperBound - previousUpperBound) * (upperToLowerBoundObjectiveValueRatio - 1.0)) / ((previousUpperToLowerBoundObjectiveValueRatio - 1.0) * (upperToLowerBoundObjectiveValueRatio - 1.0) * (upperToPreviousUpperBoundObjectiveValueRatio - 1.0));
        } else {
          stepSize = 2.0 * middleOfBounds * upperToPreviousUpperBoundObjectiveValueRatio / (1 - upperToPreviousUpperBoundObjectiveValueRatio);
        }

        // Reverts to the Bisection method, if either the step is too large (and the approximation not trusted), or lower then half our previous improvement.
        if (std::abs(stepSize) > 1.5 * std::abs(middleOfBounds) || std::abs(stepSize) < std::abs(upperBound - previousUpperBound) / 2.0) {
          stepSize = middleOfBounds;
        }
      }

      previousUpperBound = upperBound;
      previousUpperBoundObjectiveValue = upperBoundObjectiveValue;

      upperBound = upperBound + stepSize;
      upperBoundObjectiveValue = objectiveFunction(upperBound);

      // Reverts the direction, if adding the step size overshoot the root.
      if (upperBoundObjectiveValue * lowerBoundObjectiveValue > 0) {
        lowerBound = previousUpperBound;
        lowerBoundObjectiveValue = previousUpperBoundObjectiveValue;
      }
    }

    return upperBound;
  }
}
