// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("randomRotationMatrix", "[probability][randomRotationMatrix]") {
  GIVEN("A number of dimensions") {
    WHEN("The number of dimensions is greater than 2") {
      THEN("Return a rotation matrix") {
        CHECK(mant::isRotationMatrix(mant::randomRotationMatrix(5)) == true);

        arma::vec::fixed<10000> angles;
        for (arma::uword n = 0; n < angles.n_elem; ++n) {
          const arma::vec& rotatedUnitVector = mant::randomRotationMatrix(2) * arma::vec({1.0, 0.0});
          angles(n) = std::atan2(rotatedUnitVector(1), rotatedUnitVector(0));
        }

        CHECK(isUniformDistributed(angles, -arma::datum::pi, arma::datum::pi) == true);
      }
    }
  }
}

SCENARIO("randomPermutationVector", "[probability][randomPermutationVector]") {
  GIVEN("A number of elements and a cycle size") {
    WHEN("The cycle size is less than the number of elements") {
      THEN("Return a uniformly and randomly distributed permutation vector") {
        CHECK(mant::isPermutationVector(mant::randomPermutationVector(10, 5), 10, 5) == true);

        arma::umat::fixed<10000, 5> permutations;
        for (arma::uword n = 0; n < permutations.n_rows; ++n) {
          permutations.row(n) = mant::randomPermutationVector(10, 5).t();
        }

        for (arma::uword n = 0; n < permutations.n_cols; ++n) {
          CHECK(isUniformDistributed(arma::conv_to<arma::vec>::from(permutations.col(n)), 0.0, 9.0) == true);
        }
      }
    }

    WHEN("The cycle size is equal to the number of elements") {
      THEN("Return a uniformly and randomly distributed permutation vector") {
        CHECK(mant::isPermutationVector(mant::randomPermutationVector(5, 5), 5, 5) == true);

        arma::umat::fixed<10000, 5> permutations;
        for (arma::uword n = 0; n < permutations.n_rows; ++n) {
          permutations.row(n) = mant::randomPermutationVector(5, 5).t();
        }

        for (arma::uword n = 0; n < permutations.n_cols; ++n) {
          CHECK(isUniformDistributed(arma::conv_to<arma::vec>::from(permutations.col(n)), 0.0, 4.0) == true);
        }
      }
    }
  }

  GIVEN("A number of elements") {
    THEN("Return a uniformly and randomly distributed permutation vector") {
      CHECK(mant::isPermutationVector(mant::randomPermutationVector(5), 5) == true);

      arma::umat::fixed<10000, 5> permutations;
      for (arma::uword n = 0; n < permutations.n_rows; ++n) {
        permutations.row(n) = mant::randomPermutationVector(5).t();
      }

      for (arma::uword n = 0; n < permutations.n_cols; ++n) {
        CHECK(isUniformDistributed(arma::conv_to<arma::vec>::from(permutations.col(n)), 0.0, 4.0) == true);
      }
    }
  }
}

SCENARIO("randomNeighbour", "[probability][randomNeighbour]") {
  GIVEN("A parameter, a minimal and a maximal distance") {
    WHEN("The minimal distance is equal to the maximal one") {
      THEN("Return a uniformly distributed neighbour") {
        arma::vec::fixed<10000> angles;
        arma::vec::fixed<10000> lengths;

        const arma::vec& parameter{1.0, -2.0};
        for (arma::uword n = 0; n < angles.n_rows; ++n) {
          const arma::vec& neighbour = parameter - mant::randomNeighbour(parameter, 2.0, 2.0);

          lengths(n) = arma::norm(neighbour);
          angles(n) = std::atan2(neighbour(1), neighbour(0));
        }

        CHECK(arma::approx_equal(lengths, arma::zeros<arma::mat>(arma::size(lengths)) + 2.0, "absdiff", ::mant::machinePrecision) == true);
        CHECK(isUniformDistributed(angles, -arma::datum::pi, arma::datum::pi) == true);
      }
    }

    WHEN("The minimal distance is less than the maximal one") {
      THEN("Return a uniformly distributed neighbour") {
        arma::vec::fixed<10000> angles;
        arma::vec::fixed<10000> lengths;

        const arma::vec& parameter{1.0, -2.0};
        for (arma::uword n = 0; n < angles.n_rows; ++n) {
          const arma::vec& neighbour = parameter - mant::randomNeighbour(parameter, 1.0, 2.0);

          lengths(n) = arma::norm(neighbour);
          angles(n) = std::atan2(neighbour(1), neighbour(0));
        }

        CHECK(isUniformDistributed(angles, -arma::datum::pi, arma::datum::pi) == true);
        CHECK(isUniformDistributed(lengths, 1.0, 2.0) == true);
      }
    }
  }

  GIVEN("A parameter and a maximal distance") {
    THEN("Return a uniformly distributed neighbour") {
      arma::vec::fixed<10000> angles;
      arma::vec::fixed<10000> lengths;

      const arma::vec& parameter{1.0, -2.0};
      for (arma::uword n = 0; n < angles.n_rows; ++n) {
        const arma::vec& neighbour = parameter - mant::randomNeighbour(parameter, 1.0, 2.0);

        lengths(n) = arma::norm(neighbour);
        angles(n) = std::atan2(neighbour(1), neighbour(0));
      }

      CHECK(isUniformDistributed(angles, -arma::datum::pi, arma::datum::pi) == true);
      CHECK(isUniformDistributed(lengths, 0.0, 2.0) == true);
    }
  }
}

SCENARIO("uniformRandomNumbers", "[probability][uniformRandomNumbers]") {
  GIVEN("A number of rows, a number of columns and a distribution") {
    WHEN("The number of rows and/or the number of columns is 0") {
      THEN("Return an empty matrix") {
        CHECK(mant::uniformRandomNumbers(0, 10, std::uniform_real_distribution<double>()).empty() == true);
        CHECK(mant::uniformRandomNumbers(10, 0, std::uniform_real_distribution<double>()).empty() == true);
        CHECK(mant::uniformRandomNumbers(0, 0, std::uniform_real_distribution<double>()).empty() == true);
      }
    }

    WHEN("The number of rows and the number of columns is greater than 0") {
      THEN("Return a matrix with each element drawn from a uniform distribution") {
        CHECK(isUniformDistributed(arma::vectorise(mant::uniformRandomNumbers(100, 100, std::uniform_real_distribution<double>(-5.0, 2.0))), -5.0, 2.0) == true);
      }
    }
  }

  GIVEN("A number of rows and a number of columns") {
    WHEN("The number of rows and/or the number of columns is 0") {
      THEN("Return an empty matrix") {
        CHECK(mant::uniformRandomNumbers(0, 10).empty() == true);
        CHECK(mant::uniformRandomNumbers(10, 0).empty() == true);
        CHECK(mant::uniformRandomNumbers(0, 0).empty() == true);
      }
    }

    WHEN("The number of rows and the number of columns is greater than 0") {
      THEN("Return a matrix with each element drawn from a uniform distribution") {
        CHECK(isUniformDistributed(arma::vectorise(mant::uniformRandomNumbers(100, 100)), 0.0, 1.0) == true);
      }
    }
  }

  GIVEN("A number of elements and a distribution") {
    THEN("Return a vector with each element drawn from a uniform distribution") {
      CHECK(isUniformDistributed(mant::uniformRandomNumbers(10000, std::uniform_real_distribution<double>(-5.0, 2.0)), -5.0, 2.0) == true);
    }
  }

  GIVEN("A number of elements") {
    THEN("Return a vector with each element drawn from a uniform distribution") {
      CHECK(isUniformDistributed(mant::uniformRandomNumbers(10000), 0.0, 1.0) == true);
    }
  }
}

SCENARIO("normalRandomNumbers", "[probability][normalRandomNumbers]") {
  GIVEN("A number of rows, a number of columns and a distribution") {
    WHEN("The number of rows and/or the number of columns is 0") {
      THEN("Return an empty matrix") {
        CHECK(mant::normalRandomNumbers(0, 10, std::normal_distribution<double>()).empty() == true);
        CHECK(mant::normalRandomNumbers(10, 0, std::normal_distribution<double>()).empty() == true);
        CHECK(mant::normalRandomNumbers(0, 0, std::normal_distribution<double>()).empty() == true);
      }
    }

    WHEN("The number of rows and the number of columns is greater than 0") {
      THEN("Return a matrix with each element drawn from a normal distribution") {
        CHECK(isNormalDistributed(arma::vectorise(mant::normalRandomNumbers(100, 100, std::normal_distribution<double>(5.0, 3.0))) - 5.0, 3.0) == true);
      }
    }
  }

  GIVEN("A number of rows and a number of columns") {
    WHEN("The number of rows and/or the number of columns is 0") {
      THEN("Return an empty matrix") {
        CHECK(mant::normalRandomNumbers(0, 10).empty() == true);
        CHECK(mant::normalRandomNumbers(10, 0).empty() == true);
        CHECK(mant::normalRandomNumbers(0, 0).empty() == true);
      }
    }

    WHEN("The number of rows and the number of columns is greater than 0") {
      THEN("Return a matrix with each element drawn from a normal distribution") {
        CHECK(isNormalDistributed(arma::vectorise(mant::normalRandomNumbers(100, 100)), 1.0) == true);
      }
    }
  }

  GIVEN("A number of elements and a distribution") {
    THEN("Return a vector with each element drawn from a normal distribution") {
      CHECK(isNormalDistributed(mant::normalRandomNumbers(10000, std::normal_distribution<double>(5.0, 3.0)) - 5.0, 3.0) == true);
    }
  }

  GIVEN("A number of elements") {
    THEN("Return a vector with each element drawn from a normal distribution") {
      CHECK(isNormalDistributed(mant::normalRandomNumbers(10000), 1.0) == true);
    }
  }
}
