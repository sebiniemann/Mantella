// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

TEST_CASE("polynomialSize", "[algebra][polynomialSize]") {
  GIVEN("A number of indeterminates and a highest degree") {
    WHEN("The number of indeterminates is 0") {
      THEN("Return 1") {
        CHECK(mant::polynomialSize(0, 3) == 1);
      }
    }

    WHEN("The highest degree is 0") {
      THEN("Return 1") {
        CHECK(mant::polynomialSize(4, 0) == 1);
      }
    }

    WHEN("The highest degree and the number of indeterminates is not 0") {
      THEN("Return the polynomial size") {
        CHECK(mant::polynomialSize(3, 2) == 10);
      }
    }

    WHEN("The polynomial size is not representable") {
      THEN("Throw an overflow error") {
        CHECK_THROWS_AS(mant::polynomialSize(100, 100), std::overflow_error);
      }
    }
  }
}

SCENARIO("polynomial", "[algebra][polynomial]") {
  GIVEN("A parameter and a highest degree") {
    WHEN("The parameter is empty") {
      THEN("Return {1}") {
        CHECK(arma::approx_equal(mant::polynomial({}, 1), arma::vec({1.0}), "absdiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("The highest degree is 0") {
      THEN("Return {1}") {
        CHECK(arma::approx_equal(mant::polynomial({2.0}, 0), arma::vec({1.0}), "absdiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("The parameter is full of zeros") {
      THEN("Return the polynomial (and don't skip any indeterminates)") {
        CHECK(arma::approx_equal(mant::polynomial({0.0, 0.0}, 2), arma::vec({0.0, 0.0, 0.0, 0.0, 0.0, 1.0}), "absdiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("The polynomial is non-empty and the highest degree is greater than 0") {
      THEN("Return the polynomial") {
        CHECK(arma::approx_equal(mant::polynomial({-2.0, 3.0}, 3), arma::vec({-8.0, 12.0, -18.0, 27.0, 4.0, -6.0, 9.0, -2.0, 3.0, 1.0}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }
}
