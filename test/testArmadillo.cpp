// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("Hash", "[armadillo][Hash]") {
  GIVEN("A parameter") {
    WHEN("The parameter is empty") {
      THEN("Throw an invalid argument") {
        CHECK_THROWS_AS(mant::Hash()({}), std::invalid_argument);
      }
    }

    WHEN("The parameter contains NaNs") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::Hash()(arma::vec({std::numeric_limits<double>::quiet_NaN()})), std::domain_error);
      }
    }

    WHEN("The parameter does not contain NaNs") {
      THEN("Throw no exception") {
        CHECK_NOTHROW(mant::Hash()(arma::vec({1.0, std::numeric_limits<double>::infinity()})));
      }
    }
  }
}

SCENARIO("IsEqual", "[armadillo][IsEqual]") {
  GIVEN("Two parameters") {
    WHEN("Both parameters are empty") {
      THEN("Return true") {
        CHECK(mant::IsEqual()({}, {}) == true);
      }
    }

    WHEN("One parameter is a subset of the other one") {
      THEN("Return false") {
        CHECK(mant::IsEqual()(arma::vec({1.0, -2.0}), arma::vec({1.0})) == false);
        CHECK(mant::IsEqual()(arma::vec({1.0}), arma::vec({1.0, -2.0})) == false);
      }
    }

    WHEN("Both parameters contain NaNs") {
      THEN("Return false") {
        CHECK(mant::IsEqual()(arma::vec({std::numeric_limits<double>::quiet_NaN()}), arma::vec({std::numeric_limits<double>::quiet_NaN()})) == false);
      }
    }

    WHEN("Both parameters are equal in size but contain different values") {
      THEN("Return false") {
        CHECK(mant::IsEqual()(arma::vec({0.0}), arma::vec({-1.0})) == false);
      }
    }

    WHEN("Both parameters are identical") {
      THEN("Return true") {
        CHECK(mant::IsEqual()(arma::vec({1.0, -std::numeric_limits<double>::infinity()}), arma::vec({1.0, -std::numeric_limits<double>::infinity()})) == true);
      }
    }
  }
}
