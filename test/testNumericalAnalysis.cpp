// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("brent", "[numericalAnalysis][brent]") {
  GIVEN("An objective function, a lower and upper bound, a maximal number of iterations and an acceptance tolerance") {
    auto objectiveFunction = [](
        const double parameter_) {
      return std::pow(parameter_, 3.0) + 2.0;
    };

    const arma::uword numberOfIterations = discreteRandomNumber() + 100;

    WHEN("The root is between the bounds") {
      THEN("Return root value") {
        const double lowerBound = -1.0 - std::pow(2.0, 1.0 / 3.0) - std::abs(continuousRandomNumber());
        const double upperBound = 1.0 - std::pow(2.0, 1.0 / 3.0) + std::abs(continuousRandomNumber());

        CHECK(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10) == Approx(-std::pow(2.0, 1.0 / 3.0)));
      }
    }

    WHEN("A bound is a root") {
      THEN("Return root value") {
        double lowerBound = -std::pow(2.0, 1.0 / 3.0);
        double upperBound = lowerBound + std::abs(continuousRandomNumber());
        CHECK(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10) == Approx(-std::pow(2.0, 1.0 / 3.0)));

        upperBound = -std::pow(2.0, 1.0 / 3.0);
        lowerBound = upperBound - std::abs(continuousRandomNumber());
        CHECK(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10) == Approx(-std::pow(2.0, 1.0 / 3.0)));
      }
    }

    WHEN("The bounds are equal and the root") {
      THEN("Return root value") {
        const double lowerBound = -std::pow(2.0, 1.0 / 3.0);
        const double upperBound = -std::pow(2.0, 1.0 / 3.0);

        CHECK(mant::brent(objectiveFunction, lowerBound, upperBound, numberOfIterations, 1e-10) == Approx(-std::pow(2.0, 1.0 / 3.0)));
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
