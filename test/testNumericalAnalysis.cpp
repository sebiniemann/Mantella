// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("brent", "[numericalAnalysis][brent]") {
  GIVEN("An objective function, a lower and upper bound and a maximal number of iterations") {
    WHEN("The root is not found within the maximal number of iterations") {
      THEN("Return an empty parameter") {
        CHECK(std::isnan(mant::brent([](const double parameter_) { return std::pow(parameter_ - 2.0, 3.0) + 10.0 * (parameter_ - 2.0); }, -5.0, 5.0, 1)) == true);
      }
    }

    WHEN("The maximal number of iterations is 0 and a bound is the root") {
      THEN("Return an empty parameter") {
        CHECK(mant::brent([](const double parameter_) { return parameter_ - 1.0; }, 1.0, 5.0, 0) == Approx(1.0));
        CHECK(mant::brent([](const double parameter_) { return parameter_ + 1.0; }, -5.0, -1.0, 0) == Approx(-1.0));
      }
    }

    WHEN("The maximal number of iterations is 0 and no bound is the root") {
      THEN("Return an empty parameter") {
        CHECK(std::isnan(mant::brent([](const double parameter_) { return parameter_; }, 1.0, 5.0, 0)) == true);
        CHECK(std::isnan(mant::brent([](const double parameter_) { return parameter_; }, -5.0, -1.0, 0)) == true);
      }
    }

    WHEN("The bounds are equal but not the root") {
      THEN("Return an empty parameter") {
        CHECK(std::isnan(mant::brent([](const double parameter_) { return parameter_; }, 5.0, 5.0, 100)) == true);
      }
    }

    WHEN("The bounds are equal and the root") {
      THEN("Return an empty parameter") {
        CHECK(mant::brent([](const double parameter_) { return parameter_ - 1.0; }, 1.0, 1.0, 100) == Approx(1.0));
      }
    }

    WHEN("The root is between both bounds") {
      THEN("Return the root") {
        CHECK(mant::brent([](const double parameter_) { return parameter_ - 1.0; }, -5.0, 5.0, 100) == Approx(1.0));
      }
    }
  }
}
