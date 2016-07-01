// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

TEST_CASE("polynomialSize", "[algebra][polynomialSize]") {
  GIVEN("A number of elements and a highest degree") {
    WHEN("The number of elements is 0") {
      THEN("Return 1") {
        CHECK(mant::polynomialSize(0, 1) == 1);
      }
    }

    WHEN("The highest degree is 0") {
      THEN("Return 1") {
        CHECK(mant::polynomialSize(1, 0) == 1);
      }
    }

    WHEN("The resulting polynomial size is overflowing") {
      THEN("Throw an overflow error") {
        CHECK_THROWS_AS(mant::polynomialSize(100, 100), std::overflow_error);
      }
    }

    WHEN("The resulting polynomial size is not overflowing") {
      THEN("Return the polynomial size") {
        CHECK(mant::polynomialSize(3, 2) == 10);
      }
    }
  }
}

SCENARIO("polynomial", "[algebra][polynomial]") {
  GIVEN("A parameter and a highest degree") {
    WHEN("The parameter is empty") {
      THEN("Return a constant polynomial") {
        CHECK(arma::approx_equal(mant::polynomial({}, 1), arma::vec({1.0}), "absdiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("The highest degree is 0") {
      THEN("Return a constant polynomial") {
        CHECK(arma::approx_equal(mant::polynomial({2.0}, 0), arma::vec({1.0}), "absdiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("The polynomial's size is overflowing") {
      THEN("Throw an overflow error") {
        CHECK_THROWS_AS(mant::polynomial({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0}, 1000), std::overflow_error);
      }
    }

    WHEN("The polynomial's size is not overflowing") {
      THEN("Return the polynomial") {
        CHECK(arma::approx_equal(mant::polynomial({-2.0, 3.0}, 3), arma::vec({-8.0, 12.0, -18.0, 27.0, 4.0, -6.0, 9.0, -2.0, 3.0, 1.0}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }
}
