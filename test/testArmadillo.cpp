// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <unordered_set>

// Mantella
#include <mantella>

SCENARIO("range", "[armadillo][range]") {
  GIVEN("Two discrete numbers [a] and [b] and a step size") {
    WHEN("[a] < [b]") {
      // Including zero
      const arma::uword a = discreteRandomNumber() - 1;
      CAPTURE(a);

      const arma::uword b = a + discreteRandomNumber();
      CAPTURE(b);

      const arma::uword stepSize = discreteRandomNumber();
      CAPTURE(stepSize);

      THEN("Return the range") {
        IS_EQUAL(mant::range(a, b, stepSize), arma::linspace<arma::Col<arma::uword>>(a, b, (b - a) / stepSize + 1));
      }
    }

    WHEN("[a] = [b]") {
      // Including zero
      const arma::uword a = discreteRandomNumber() - 1;
      CAPTURE(a);

      const arma::uword b = a;
      CAPTURE(b);

      const arma::uword stepSize = discreteRandomNumber();
      CAPTURE(stepSize);

      THEN("Return only a (or b)") {
        IS_EQUAL(mant::range(a, b, stepSize), {a});
      }
    }

    WHEN("[a] > [b]") {
      const arma::uword a = 4;
      CAPTURE(a);

      const arma::uword b = 1;
      CAPTURE(b);

      const arma::uword stepSize = 2;
      CAPTURE(stepSize);

      THEN("Return the range") {
        IS_EQUAL(mant::range(a, b, stepSize), {4, 2});
      }
    }
  }

  GIVEN("Two discrete numbers [a] and [b]") {
    WHEN("[a] < [b]") {
      // Including zero
      const arma::uword a = discreteRandomNumber() - 1;
      CAPTURE(a);

      const arma::uword b = a + discreteRandomNumber();
      CAPTURE(b);

      THEN("Return the range") {
        IS_EQUAL(mant::range(a, b), arma::linspace<arma::Col<arma::uword>>(a, b, b - a + 1));
      }
    }

    WHEN("[a] = [b]") {
      // Including zero
      const arma::uword a = discreteRandomNumber() - 1;
      CAPTURE(a);

      const arma::uword b = a;
      CAPTURE(b);

      THEN("Return only a (or b)") {
        IS_EQUAL(mant::range(a, b), {a});
      }
    }

    WHEN("[a] > [b]") {
      const arma::uword a = 4;
      CAPTURE(a);

      const arma::uword b = 1;
      CAPTURE(b);

      const arma::uword stepSize = 2;
      CAPTURE(stepSize);

      arma::Col<arma::uword> range((a - b) / stepSize + 1);
      for (arma::uword n = 0; n < range.n_elem; ++n) {
        // Calculates the next element from scratch (instead of increasing it step by step), to reduce rounding errors.
        range(n) = a - stepSize * n;
      }

      THEN("Return the range") {
        IS_EQUAL(mant::range(a, b, stepSize), range);
      }
    }
  }
}

SCENARIO("Hash", "[armadillo][Hash]") {
  GIVEN("One million 5-dimensional parameters") {
    mant::Hash hash;

    const arma::uword numberOfParameters = 1000000;
    CAPTURE(numberOfParameters);

    const arma::uword numberOfDimensions = 5;
    CAPTURE(numberOfParameters);

    WHEN("All parameters are randomly and uniformly distributed") {
      const arma::Mat<double>& parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);

      THEN("There is at most one hash collision") {
        std::unordered_set<arma::uword> hashes;

        unsigned int numberOfCollisions = 0;
        for (unsigned int n = 0; n < parameters.n_cols; ++n) {
          auto result = hashes.insert(hash(parameters.col(n)));
          if (!result.second) {
            ++numberOfCollisions;
          }
        }

        CHECK(numberOfCollisions < 2);
      }
    }
  }
}

SCENARIO("IsEqual", "[armadillo][IsEqual]") {
  GIVEN("Two vectors") {
    mant::IsEqual isEqual;

    const arma::uword numberOfElements = discreteRandomNumber();
    CAPTURE(numberOfElements);

    const arma::Col<double>& firstParameter = continuousRandomNumbers(numberOfElements);
    CAPTURE(firstParameter);

    WHEN("Both are identical (having the same elements in the same order)") {
      const arma::Col<double>& secondParameter = firstParameter;
      CAPTURE(secondParameter);

      THEN("Return true") {
        CHECK(isEqual(firstParameter, secondParameter) == true);
      }
    }

    WHEN("Both have the same number of elements but at least one different element") {
      arma::Col<double> secondParameter = firstParameter;
      secondParameter(0) += 1;
      CAPTURE(secondParameter);

      THEN("Return false") {
        CHECK(isEqual(firstParameter, secondParameter) == false);
      }
    }

    WHEN("Both have a different number of elements") {
      const arma::uword differentNumberOfDimensions = differentDiscreteRandomNumber(numberOfElements);
      CAPTURE(differentNumberOfDimensions);

      const arma::Col<double>& secondParameter = continuousRandomNumbers(differentNumberOfDimensions);
      CAPTURE(secondParameter);

      THEN("Return false") {
        CHECK(isEqual(firstParameter, secondParameter) == false);
      }
    }
  }
}
