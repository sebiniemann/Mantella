// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("brent", "[numericalAnalysis][brent]") {
  GIVEN("An objective function, a lower and upper bound, a maximal number of iterations and an acceptance tolerance") {
    auto objectiveFunction = [](
        const double parameter_) {
      return (parameter_ - 1.0) * std::pow(parameter_ + 1.0, 2.0) + 2.0;
    };
    double objectiveFunctionRoot = (1.0 / 3.0) * (-1.0 - (4.0 / std::pow(19.0 - 3.0 * std::sqrt(33.0), 1.0 / 3.0)) - std::pow(19.0 - 3.0 * std::sqrt(33.0), 1.0 / 3.0));

    const arma::uword numberOfIterations = discreteRandomNumber() + 100;

    WHEN("The root is between the bounds") {
      THEN("Return root value") {
        const double lowerBound = objectiveFunctionRoot - 1.0 - std::abs(continuousRandomNumber());
        const double upperBound = objectiveFunctionRoot + 1.0 + std::abs(continuousRandomNumber());

        CHECK(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10) == Approx(objectiveFunctionRoot));
      }
    }

    WHEN("A bound is a root") {
      THEN("Return root value") {
        double lowerBound = objectiveFunctionRoot;
        double upperBound = lowerBound + std::abs(continuousRandomNumber());
        CHECK(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10) == Approx(objectiveFunctionRoot));

        upperBound = objectiveFunctionRoot;
        lowerBound = upperBound - std::abs(continuousRandomNumber());
        CHECK(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10) == Approx(objectiveFunctionRoot));
      }
    }

    WHEN("The bounds are equal and the root") {
      THEN("Return root value") {
        const double lowerBound = objectiveFunctionRoot;
        const double upperBound = objectiveFunctionRoot;

        CHECK(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10) == Approx(objectiveFunctionRoot));
      }
    }

    WHEN("The objective function is not callable") {
      THEN("Throw a std::logic_error") {
        const double lowerBound = continuousRandomNumber();
        const double upperBound = lowerBound + std::abs(continuousRandomNumber()) + 1.0;

        CHECK_THROWS_AS(mant::brent(nullptr, lowerBound, upperBound, numberOfIterations, 1e-10), std::logic_error);
      }
    }

    WHEN("If the acceptanceTolerance is negative") {
      THEN("Throw a std::logic_error") {
        const double lowerBound = continuousRandomNumber();
        const double upperBound = lowerBound + std::abs(continuousRandomNumber()) + 1.0;

        CHECK_THROWS_AS(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, -1e-10), std::logic_error);
      }
    }

    WHEN("The lower bound is strict greater than the upper bound") {
      THEN("Throw a std::logic_error") {
        const double lowerBound = continuousRandomNumber();
        const double upperBound = lowerBound - std::abs(continuousRandomNumber()) - 1.0;

        CHECK_THROWS_AS(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10), std::logic_error);
      }
    }

    WHEN("The bounds are equal and there is no root in between") {
      THEN("Throw a std::logic_error") {
        const double lowerBound = std::abs(continuousRandomNumber());
        const double upperBound = lowerBound;

        CHECK_THROWS_AS(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10), std::logic_error);
      }
    }

    WHEN("If the objective values of the bound values have the same sign") {
      THEN("Throw a std::logic_error") {
        const double lowerBound = std::abs(continuousRandomNumber());
        const double upperBound = lowerBound + std::abs(continuousRandomNumber()) + 1.0;

        CHECK_THROWS_AS(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10), std::logic_error);
      }
    }
  }
}
