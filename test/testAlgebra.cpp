// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("polynomialSize", "[algebra][polynomialSize]") {
  GIVEN("A number of elements and a polynomial order") {
    WHEN("The number of elements is 0") {
      const arma::uword numberOfElements = 0;
      CAPTURE(numberOfElements);

      // Includes 0
      const arma::uword polynomialOrder = discreteRandomNumber() - 1;
      CAPTURE(polynomialOrder);

      THEN("Return 1") {
        CHECK(mant::polynomialSize(numberOfElements, polynomialOrder) == 1);
      }
    }

    WHEN("The number of elements is 1") {
      const arma::uword numberOfElements = 1;
      CAPTURE(numberOfElements);

      // Includes 0
      const arma::uword polynomialOrder = discreteRandomNumber() - 1;
      CAPTURE(polynomialOrder);

      THEN("Return 1 + [polynomialOrder]") {
        CHECK(mant::polynomialSize(numberOfElements, polynomialOrder) == 1 + polynomialOrder);
      }
    }

    WHEN("The number of elements is at least 2") {
      const arma::uword numberOfElements = 1 + discreteRandomNumber();
      CAPTURE(numberOfElements);

      AND_WHEN("The polynomial order is 0") {
        const arma::uword polynomialOrder = 0;
        CAPTURE(polynomialOrder);

        THEN("Return 1") {
          CHECK(mant::polynomialSize(numberOfElements, polynomialOrder) == 1);
        }
      }

      AND_WHEN("The polynomial order is 1") {
        const arma::uword polynomialOrder = 1;
        CAPTURE(polynomialOrder);

        THEN("Return 1 + [numberOfElements]") {
          CHECK(mant::polynomialSize(numberOfElements, polynomialOrder) == 1 + numberOfElements);
        }
      }

      AND_WHEN("The polynomial order is 2") {
        const arma::uword polynomialOrder = 2;
        CAPTURE(polynomialOrder);

        THEN("Return [numberOfElements] * ([numberOfElements] + 3) / 2 + 1") {
          CHECK(mant::polynomialSize(numberOfElements, polynomialOrder) == numberOfElements * (numberOfElements + 3) / 2 + 1);
        }
      }

      AND_WHEN("The polynomial order has a value greater than 2") {
        const arma::uword polynomialOrder = 2 + discreteRandomNumber();
        CAPTURE(polynomialOrder);

        THEN("Return the summed number of combinations for each degree") {
          arma::uword polynomialSize = 1;
          for (arma::uword n = 1; n <= polynomialOrder; ++n) {
            polynomialSize += mant::nchoosek(numberOfElements + n - 1, n);
          }

          CHECK(mant::polynomialSize(numberOfElements, polynomialOrder) == polynomialSize);
        }
      }
    }
  }
}

SCENARIO("polynomial", "[algebra][polynomial]") {
  GIVEN("A parameter and a polynomial order") {
    const arma::Col<double>& parameter = {2, -3.4};
    CAPTURE(parameter);

    WHEN("The polynomial order is 0") {
      const arma::uword polynomialOrder = 0;
      CAPTURE(polynomialOrder);

      THEN("Return the single constant 1") {
        IS_EQUAL(mant::polynomial(parameter, polynomialOrder), {1});
      }
    }

    WHEN("The polynomial order is 1") {
      const arma::uword polynomialOrder = 1;
      CAPTURE(polynomialOrder);

      THEN("Return a linear polynomial") {
        HAS_SAME_ELEMENTS(mant::polynomial(parameter, polynomialOrder), {2, -3.4 /* Linear */, 1 /* Constant */});
      }
    }

    WHEN("The polynomial order is 2") {
      const arma::uword polynomialOrder = 2;
      CAPTURE(polynomialOrder);

      THEN("Return a quadratic polynomial") {
        HAS_SAME_ELEMENTS(mant::polynomial(parameter, polynomialOrder), {4, -6.8, 11.56 /* Quadratic */, 2, -3.4 /* Linear */, 1 /* Constant */});
      }
    }

    WHEN("The polynomial order is 3") {
      const arma::uword polynomialOrder = 3;
      CAPTURE(polynomialOrder);

      THEN("Return a cubic polynomial") {
        HAS_SAME_ELEMENTS(mant::polynomial(parameter, polynomialOrder), {8, -13.6, 23.12, -39.304 /* Cubic */, 4, -6.8, 11.56 /* Quadratic */, 2, -3.4 /* Linear */, 1 /* Constant */});
      }
    }

    WHEN("The polynomial order is 5") {
      const arma::uword polynomialOrder = 5;
      CAPTURE(polynomialOrder);

      THEN("Return a 5-dimensional polynomial") {
        HAS_SAME_ELEMENTS(mant::polynomial(parameter, polynomialOrder), {32, -54.4, 92.48, -157.216, 267.2672, -454.35424 /* 5-dimensional */, 16, -27.2, 46.24, -78.608, 133.6336 /* 4-dimensional */, 8, -13.6, 23.12, -39.304 /* Cubic */, 4, -6.8, 11.56 /* Quadratic */, 2, -3.4 /* Linear */, 1 /* Constant */});
      }
    }
  }
}
