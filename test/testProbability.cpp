// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("randomRotationMatrix", "[probability][randomRotationMatrix]") {
  GIVEN("A number of dimensions") {
    WHEN("The number of dimensions is 0") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::randomRotationMatrix(0), std::domain_error);
      }
    }

    WHEN("The number of dimensions is less than 2") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::randomRotationMatrix(1), std::domain_error);
      }
    }

    WHEN("The number of dimensions is greater than 2") {
      THEN("Return a rotation matrix") {
        CHECK(mant::isRotationMatrix(mant::randomRotationMatrix(5)) == true);

        arma::mat::fixed<100000, 1> angles;
        for (arma::uword n = 0; n < angles.n_elem; ++n) {
          const arma::vec& rotatedUnitVector = mant::randomRotationMatrix(2) * arma::vec({1.0, 0.0});
          angles(n) = std::atan2(rotatedUnitVector(1), rotatedUnitVector(0));
        }

        CHECK(isUniformlyDistributed(angles, -arma::datum::pi, arma::datum::pi) == true);
      }
    }
  }
}

SCENARIO("randomPermutationVector", "[probability][randomPermutationVector]") {
  GIVEN("A number of elements and a cycle size") {
    WHEN("The number of elements is 0") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::randomPermutationVector(0, 2), std::domain_error);
      }
    }

    WHEN("The cycle size is 0") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::randomPermutationVector(2, 0), std::domain_error);
      }
    }

    WHEN("The cycle size is greater than the number of elements") {
      THEN("Throw a logic error") {
        CHECK_THROWS_AS(mant::randomPermutationVector(2, 3), std::logic_error);
      }
    }

    WHEN("The cycle size is less than the number of elements") {
      THEN("Return a uniformly and randomly distributed permutation vector") {
        CHECK(mant::isPermutationVector(mant::randomPermutationVector(10, 5), 10, 5) == true);

        arma::umat::fixed<100000, 5> permutations;
        for (arma::uword n = 0; n < permutations.n_rows; ++n) {
          permutations.row(n) = mant::randomPermutationVector(10, 5).t();
        }

        CHECK(isUniformlyDistributed(permutations, 0, 9) == true);
      }
    }

    WHEN("The cycle size is equal to the number of elements") {
      THEN("Return a uniformly and randomly distributed permutation vector") {
        CHECK(mant::isPermutationVector(mant::randomPermutationVector(5, 5), 5, 5) == true);

        arma::umat::fixed<100000, 5> permutations;
        for (arma::uword n = 0; n < permutations.n_rows; ++n) {
          permutations.row(n) = mant::randomPermutationVector(5, 5).t();
        }

        CHECK(isUniformlyDistributed(permutations, 0, 4) == true);
      }
    }
  }
}

SCENARIO("randomNeighbour", "[probability][randomNeighbour]") {
  GIVEN("A parameter, a minimal and a maximal distance") {
    WHEN("The parameter is empty") {
      THEN("Throw a invalid argument error") {
        CHECK_THROWS_AS(mant::randomNeighbour({}, 0.0, 1.0), std::invalid_argument);
      }
    }

    WHEN("A distance is negative") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::randomNeighbour(arma::vec({1.0}), -1.0, 1.0), std::domain_error);
        CHECK_THROWS_AS(mant::randomNeighbour(arma::vec({1.0}), -2.0, -1.0), std::domain_error);
      }
    }

    WHEN("The minimal distance is greater than the maximal one") {
      THEN("Throw a logic error") {
        CHECK_THROWS_AS(mant::randomNeighbour(arma::vec({1.0}), 1.0, 0.0), std::logic_error);
      }
    }

    WHEN("The minimal distance is equal to the maximal one") {
      THEN("Return a uniformly distributed neighbour") {
        arma::mat::fixed<100000, 1> angles;
        arma::mat::fixed<100000, 1> lengths;

        const arma::vec& parameter({1.0, -2.0});
        for (arma::uword n = 0; n < angles.n_rows; ++n) {
          const arma::vec& neighbour = parameter - mant::randomNeighbour(parameter, 2.0, 2.0);

          lengths(n) = arma::norm(neighbour);
          angles(n) = std::atan2(neighbour(1), neighbour(0));
        }

        CHECK(arma::approx_equal(lengths, arma::zeros<arma::mat>(arma::size(lengths)) + 2.0, "absdiff", ::mant::machinePrecision) == true);
        CHECK(isUniformlyDistributed(angles, -arma::datum::pi, arma::datum::pi) == true);
      }
    }

    WHEN("The minimal distance is less than the maximal one") {
      THEN("Return a uniformly distributed neighbour") {
        arma::mat::fixed<100000, 1> angles;
        arma::mat::fixed<100000, 1> lengths;

        const arma::vec& parameter({1.0, -2.0});
        for (arma::uword n = 0; n < angles.n_rows; ++n) {
          const arma::vec& neighbour = parameter - mant::randomNeighbour(parameter, 1.0, 2.0);

          lengths(n) = arma::norm(neighbour);
          angles(n) = std::atan2(neighbour(1), neighbour(0));
        }

        CHECK(isUniformlyDistributed(angles, -arma::datum::pi, arma::datum::pi) == true);
        CHECK(isUniformlyDistributed(lengths, 1.0, 2.0) == true);
      }
    }
  }
}
