// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("brent", "[numericalAnalysis][brent]") {
  GIVEN("An objective function, a lower and upper bound and a maximal number of iterations") {
    WHEN("The objective function is not callable") {
      THEN("Throw an invalid argument error") {
        CHECK_THROWS_AS(mant::brent(nullptr, -5.0, 5.0, 100), std::invalid_argument);
      }
    }

    WHEN("A bound is NaN") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::brent([](const double parameter_) { return parameter_; }, std::numeric_limits<double>::quiet_NaN(), 5.0, 100), std::domain_error);
        CHECK_THROWS_AS(mant::brent([](const double parameter_) { return parameter_; }, -5.0, std::numeric_limits<double>::quiet_NaN(), 100), std::domain_error);
      }
    }

    WHEN("A bound is +/-infinity") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::brent([](const double parameter_) { return parameter_; }, std::numeric_limits<double>::infinity(), 5.0, 100), std::domain_error);
        CHECK_THROWS_AS(mant::brent([](const double parameter_) { return parameter_; }, -5.0, std::numeric_limits<double>::infinity(), 100), std::domain_error);
      }
    }

    WHEN("The lower bound is greater than the upper bound") {
      THEN("Throw a logic error") {
        CHECK_THROWS_AS(mant::brent([](const double parameter_) { return parameter_; }, 5.0, -5.0, 100), std::logic_error);
      }
    }

    WHEN("The bounds objective value has the same sign") {
      THEN("Return an invalid argument error") {
        CHECK_THROWS_AS(mant::brent([](const double parameter_) { return parameter_; }, -5.0, -4.0, 100), std::invalid_argument);
        CHECK_THROWS_AS(mant::brent([](const double parameter_) { return parameter_; }, 4.0, 5.0, 100), std::invalid_argument);
      }
    }

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
