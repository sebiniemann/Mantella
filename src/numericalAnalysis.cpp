#include "mantella_bits/numericalAnalysis.hpp"

// C++ standard library
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

// Mantella
#include "mantella_bits/config.hpp"

namespace mant {
  // To understand the following equations, we advise to carefully read the following paper:
  // @see R. P. Brent (1971). An algorithm with guaranteed convergence for finding a zero of a function. The Computer Journal, 14(4), pp. 422-425.
  double brent(
      std::function<double(
          double)> objectiveFunction,
      double lowerBound,
      double upperBound,
      arma::uword maximalNumberOfIterations) {
    assert(static_cast<bool>(objectiveFunction) && "brent: The objective function must be callable.");
    assert(std::isfinite(lowerBound) && "brent: The lower bound must not be finite.");
    assert(std::isfinite(upperBound) && "brent: The upper bound must not be finite.");
    assert(lowerBound <= upperBound && "brent: The lower bound must be less than or equal to the upper bound.");

    double lowerBoundObjectiveValue = objectiveFunction(lowerBound);

    if (std::abs(lowerBound - upperBound) < ::mant::machinePrecision) {
      // Both bounds are too close ...
      if (std::abs(lowerBoundObjectiveValue) >= ::mant::machinePrecision) {
        // ... but are not close enough to the root.
        return std::numeric_limits<double>::quiet_NaN();
      } else {
        // ... and are close enough to the root.
        return lowerBound;
      }
    }

    if (std::abs(lowerBoundObjectiveValue) < ::mant::machinePrecision) {
      // The lower bound is already close enough to the root.
      return lowerBound;
    }

    double upperBoundObjectiveValue = objectiveFunction(upperBound);
    if (std::abs(upperBoundObjectiveValue) < ::mant::machinePrecision) {
      // The upper bound is already close enough to the root.
      return upperBound;
    }

    if (maximalNumberOfIterations == 0) {
      return std::numeric_limits<double>::quiet_NaN();
    }

    assert(std::signbit(lowerBoundObjectiveValue) != std::signbit(upperBoundObjectiveValue) && "brent: The lower bound and upper bound objective value must have a different sign.");

    // The previous upper bound is used within the interpolation and **must** be different from the current upper bound.
    // Therefore, the initial, previous upper bound is set to the lower bound.
    double previousUpperBound = lowerBound;
    double previousUpperBoundObjectiveValue = lowerBoundObjectiveValue;

    for (arma::uword numberOfIterations = 0; numberOfIterations < maximalNumberOfIterations && std::abs(upperBoundObjectiveValue) >= ::mant::machinePrecision; ++numberOfIterations) {
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
      // In case the assumption that "a lower value is closer to the root" is incorrect, the interpolation is skipped and a bisection is performed.
      if (std::abs(previousUpperBoundObjectiveValue) <= std::abs(upperBoundObjectiveValue) || std::isinf(lowerBoundObjectiveValue) || std::isinf(upperBoundObjectiveValue)) {
        stepSize = middleOfBounds;
      } else {
        double upperToPreviousUpperBoundObjectiveValueRatio = upperBoundObjectiveValue / previousUpperBoundObjectiveValue;
        // Uses an inverse quadratic interpolation if we got 3 different points and otherwise a linear instead.
        if (std::abs(previousUpperBound - lowerBound) > 0.0) {
          stepSize = -(upperToPreviousUpperBoundObjectiveValueRatio * (2.0 * middleOfBounds * previousUpperBoundObjectiveValue * (previousUpperBoundObjectiveValue - upperBoundObjectiveValue) + lowerBoundObjectiveValue * (lowerBoundObjectiveValue - upperBoundObjectiveValue) * (upperBound - previousUpperBound))) / ((upperToPreviousUpperBoundObjectiveValueRatio - 1.0) * (previousUpperBoundObjectiveValue - lowerBoundObjectiveValue) * (lowerBoundObjectiveValue - upperBoundObjectiveValue));
        } else {
          stepSize = -2.0 * middleOfBounds * upperToPreviousUpperBoundObjectiveValueRatio / (upperToPreviousUpperBoundObjectiveValueRatio - 1.0);
        }

        // Reverts to the Bisection method, if either the step is too large (and the approximation not trusted), or lower than half our previous improvement.
        if (std::abs(stepSize) > 1.5 * std::abs(middleOfBounds) || std::abs(stepSize) < std::abs(upperBound - previousUpperBound) / 2.0) {
          stepSize = middleOfBounds;
        }
      }

      previousUpperBound = upperBound;
      previousUpperBoundObjectiveValue = upperBoundObjectiveValue;

      upperBound = upperBound + stepSize;
      upperBoundObjectiveValue = objectiveFunction(upperBound);

      // Reverts the direction, if adding the step size overshoot the root.
      if (std::signbit(upperBoundObjectiveValue) == std::signbit(lowerBoundObjectiveValue)) {
        lowerBound = previousUpperBound;
        lowerBoundObjectiveValue = previousUpperBoundObjectiveValue;
      }
    }

    if (std::abs(upperBoundObjectiveValue) >= ::mant::machinePrecision) {
      return std::numeric_limits<double>::quiet_NaN();
    } else {
      return upperBound;
    }
  }
}
