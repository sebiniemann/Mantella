// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("randomRotationMatrix", "[probability][randomRotationMatrix]") {
  GIVEN("A number of dimensions") {
    WHEN("The number of dimensions is 2") {
      const arma::uword numberOfDimensions = 2;
      CAPTURE(numberOfDimensions);

      THEN("Return a uniformly and randomly distributed 2-dimensional rotation matrix") {
        arma::Col<double>::fixed<10000> angles;
        for (arma::uword n = 0; n < angles.n_elem; ++n) {
          const arma::Mat<double>::fixed<2, 2>& rotationMatrix = mant::randomRotationMatrix(numberOfDimensions);
          CAPTURE(rotationMatrix);
          CHECK(mant::isRotationMatrix(rotationMatrix));

          // Rotates a fixed vector with length 1, to measure the angles based on the same vector each time.
          const arma::Col<double>::fixed<2>& rotatedUnitVector = rotationMatrix * arma::normalise(arma::ones<arma::Col<double>>(numberOfDimensions));
          angles(n) = std::atan2(rotatedUnitVector(1), rotatedUnitVector(0));
        }
        CAPTURE(angles);

        IS_UNIFORM(angles, -arma::datum::pi, arma::datum::pi);
      }
    }

    WHEN("The number of dimensions is 3") {
      const arma::uword numberOfDimensions = 3;
      CAPTURE(numberOfDimensions);

      THEN("Return a uniformly and randomly distributed 3-dimensional rotation matrix") {
        arma::Col<double>::fixed<10000> rollAngles;
        arma::Col<double>::fixed<10000> pitchAngles;
        arma::Col<double>::fixed<10000> yawAngles;
        for (arma::uword n = 0; n < rollAngles.n_elem; ++n) {
          const arma::Mat<double>::fixed<3, 3>& rotationMatrix = mant::randomRotationMatrix(numberOfDimensions);
          CAPTURE(rotationMatrix);
          CHECK(mant::isRotationMatrix(rotationMatrix));

          // Rotates a fixed vector with length 1, to measure the angles based on the same vector each time.
          const arma::Col<double>::fixed<3>& rotatedUnitVector = rotationMatrix * arma::normalise(arma::ones<arma::Col<double>>(numberOfDimensions));
          rollAngles(n) = std::atan2(rotatedUnitVector(1), rotatedUnitVector(0));
          pitchAngles(n) = std::atan2(rotatedUnitVector(2), rotatedUnitVector(1));
          yawAngles(n) = std::atan2(rotatedUnitVector(0), rotatedUnitVector(2));
        }
        CAPTURE(rollAngles);
        CAPTURE(pitchAngles);
        CAPTURE(yawAngles);

        IS_UNIFORM(rollAngles, -arma::datum::pi, arma::datum::pi);
        IS_UNIFORM(pitchAngles, -arma::datum::pi, arma::datum::pi);
        IS_UNIFORM(yawAngles, -arma::datum::pi, arma::datum::pi);
      }
    }

    WHEN("The number of dimensions is greater than 3") {
      // Limits the number of dimensions to 10, calculating 55 different angles should be enough.
      const arma::uword numberOfDimensions = std::min(static_cast<arma::uword>(10), 3 + getDiscreteRandomNumber());
      CAPTURE(numberOfDimensions);

      THEN("Return a uniformly and randomly distributed n-dimensional rotation matrix") {
        arma::Mat<double> angles(10000, mant::nchoosek(numberOfDimensions, 2));

        for (arma::uword n = 0; n < angles.n_rows; ++n) {
          const arma::Mat<double>& rotationMatrix = mant::randomRotationMatrix(numberOfDimensions);
          CHECK(mant::isRotationMatrix(rotationMatrix));

          const arma::Col<double>& rotatedUnitVector = rotationMatrix * arma::normalise(arma::ones<arma::Col<double>>(numberOfDimensions));
          CHECK(rotatedUnitVector.n_elem == numberOfDimensions);

          arma::uword k = 0;
          for (arma::uword l = 0; l < rotatedUnitVector.n_elem; ++l) {
            for (arma::uword m = l + 1; m < rotatedUnitVector.n_elem; ++m) {
              angles(n, k++) = std::atan2(rotatedUnitVector(l), rotatedUnitVector(m));
            }
          }
        }
        CAPTURE(angles);

        for (arma::uword n = 0; n < angles.n_cols; ++n) {
          IS_UNIFORM(angles.col(n), -arma::datum::pi, arma::datum::pi);
        }
      }
    }
  }
}

SCENARIO("randomPermutationVector", "[probability][randomPermutationVector]") {
  GIVEN("A number of elements and a cycle size") {
    WHEN("The cycle size is less than the number of elements") {
      const arma::uword cycleSize = getDiscreteRandomNumber();
      CAPTURE(cycleSize);

      const arma::uword numberOfElements = cycleSize + getDiscreteRandomNumber();
      CAPTURE(numberOfElements);

      THEN("Return a uniformly and randomly distributed permutation vector") {
        arma::Mat<arma::uword> permutations(10000, cycleSize);
        for (arma::uword n = 0; n < permutations.n_rows; ++n) {
          const arma::Col<arma::uword>& permutationVector = mant::randomPermutationVector(numberOfElements, cycleSize);
          CAPTURE(permutationVector);
          CHECK(mant::isPermutationVector(permutationVector, numberOfElements, cycleSize));

          permutations.row(n) = permutationVector.t();
        }

        for (arma::uword n = 0; n < permutations.n_cols; ++n) {
          CAPTURE(permutations.col(n));
          IS_UNIFORM(permutations.col(n), 0, numberOfElements - 1);
        }
      }
    }

    WHEN("The cycle size is equal to the number of elements") {
      const arma::uword numberOfElements = getDiscreteRandomNumber();
      CAPTURE(numberOfElements);

      const arma::uword cycleSize = numberOfElements;
      CAPTURE(cycleSize);

      THEN("Return a uniformly and randomly distributed permutation vector") {
        arma::Mat<arma::uword> permutations(10000, cycleSize);
        for (arma::uword n = 0; n < permutations.n_rows; ++n) {
          const arma::Col<arma::uword>& permutationVector = mant::randomPermutationVector(numberOfElements, cycleSize);
          CAPTURE(permutationVector);
          CHECK(mant::isPermutationVector(permutationVector, numberOfElements, cycleSize));

          permutations.row(n) = permutationVector.t();
        }

        for (arma::uword n = 0; n < permutations.n_cols; ++n) {
          CAPTURE(permutations.col(n));
          IS_UNIFORM(permutations.col(n), 0, numberOfElements - 1);
        }
      }
    }

    WHEN("The cycle size is greater than the number of elements") {
      const arma::uword numberOfElements = getDiscreteRandomNumber();
      CAPTURE(numberOfElements);

      const arma::uword cycleSize = numberOfElements + getDiscreteRandomNumber();
      CAPTURE(cycleSize);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::randomPermutationVector(numberOfElements, cycleSize), std::logic_error);
      }
    }
  }

  GIVEN("A number of elements") {
    const arma::uword numberOfElements = getDiscreteRandomNumber();
    CAPTURE(numberOfElements);

    THEN("Return a uniformly and randomly distributed permutation vector") {
      arma::Mat<arma::uword> permutations(10000, numberOfElements);
      for (arma::uword n = 0; n < permutations.n_rows; ++n) {
        const arma::Col<arma::uword>& permutationVector = mant::randomPermutationVector(numberOfElements);
        CAPTURE(permutationVector);
        CHECK(mant::isPermutationVector(permutationVector, numberOfElements, numberOfElements));

        permutations.row(n) = permutationVector.t();
      }

      for (arma::uword n = 0; n < permutations.n_cols; ++n) {
        CAPTURE(permutations.col(n));
        IS_UNIFORM(permutations.col(n), 0, numberOfElements - 1);
      }
    }
  }
}

SCENARIO("randomNeighbour", "[probability][randomNeighbour]") {
  GIVEN("A parameter, a minimal and a maximal distance") {
    const arma::uword numberOfElements = getDiscreteRandomNumber();
    CAPTURE(numberOfElements);

    const arma::Col<double>& parameter = getContinuousRandomNumbers(numberOfElements);
    CAPTURE(parameter);

    WHEN("The maximal distance greater than 0") {
      AND_WHEN("The maximal distance is greater than the minimal distance") {
        const double minimalDistance = std::abs(getContinuousRandomNumber());
        CAPTURE(minimalDistance);

        const double maximalDistance = minimalDistance + std::abs(getContinuousRandomNumber()) + 1.0;
        CAPTURE(maximalDistance);

        THEN("Return a uniformly and randomly distributed parameter") {
          arma::Mat<double> angles(10000, mant::nchoosek(parameter.n_elem, 2));
          arma::Col<double>::fixed<10000> lengths;

          for (arma::uword n = 0; n < angles.n_rows; ++n) {
            const arma::Col<double>& neighbour = parameter - mant::randomNeighbour(parameter, minimalDistance, maximalDistance);
            CHECK(neighbour.n_elem == numberOfElements);

            lengths(n) = arma::norm(neighbour);

            arma::uword k = 0;
            for (arma::uword l = 0; l < neighbour.n_elem; ++l) {
              for (arma::uword m = l + 1; m < neighbour.n_elem; ++m) {
                angles(n, k++) = std::atan2(neighbour(l), neighbour(m));
              }
            }
          }
          CAPTURE(angles);
          CAPTURE(lengths);

          for (arma::uword n = 0; n < angles.n_cols; ++n) {
            IS_UNIFORM(angles.col(n), -arma::datum::pi, arma::datum::pi);
          }
          IS_UNIFORM(lengths, minimalDistance, maximalDistance);
        }
      }

      AND_WHEN("The maximal distance is equal to the minimal distance") {
        const double minimalDistance = std::abs(getContinuousRandomNumber()) + 1.0;
        CAPTURE(minimalDistance);

        const double maximalDistance = minimalDistance;
        CAPTURE(maximalDistance);

        THEN("Return a uniformly and randomly distributed parameter") {
          arma::Mat<double> angles(10000, mant::nchoosek(parameter.n_elem, 2));
          arma::Col<double>::fixed<10000> lengths;

          for (arma::uword n = 0; n < angles.n_rows; ++n) {
            const arma::Col<double>& neighbour = parameter - mant::randomNeighbour(parameter, minimalDistance, maximalDistance);
            CHECK(neighbour.n_elem == numberOfElements);

            CHECK(arma::norm(neighbour) == Approx(minimalDistance));

            arma::uword k = 0;
            for (arma::uword l = 0; l < neighbour.n_elem; ++l) {
              for (arma::uword m = l + 1; m < neighbour.n_elem; ++m) {
                angles(n, k++) = std::atan2(neighbour(l), neighbour(m));
              }
            }
          }
          CAPTURE(angles);

          for (arma::uword n = 0; n < angles.n_cols; ++n) {
            IS_UNIFORM(angles.col(n), -arma::datum::pi, arma::datum::pi);
          }
        }
      }
    }

    WHEN("The maximal distance is 0") {
      AND_WHEN("The maximal distance is equal to the minimal distance") {
        const double minimalDistance = 0.0;
        CAPTURE(minimalDistance);

        const double maximalDistance = 0.0;
        CAPTURE(maximalDistance);

        THEN("Return the parameter") {
          IS_EQUAL(mant::randomNeighbour(parameter, minimalDistance, maximalDistance), parameter);
        }
      }
    }

    WHEN("The minimal distance is negative") {
      const double maximalDistance = -std::abs(getContinuousRandomNumber());
      CAPTURE(maximalDistance);

      const double minimalDistance = std::abs(getContinuousRandomNumber());
      CAPTURE(minimalDistance);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::randomNeighbour(parameter, minimalDistance, maximalDistance), std::logic_error);
      }
    }

    WHEN("The maximal distance is less than the minimal distance") {
      const double maximalDistance = std::abs(getContinuousRandomNumber());
      CAPTURE(maximalDistance);

      const double minimalDistance = maximalDistance + std::abs(getContinuousRandomNumber()) + 1.0;
      CAPTURE(minimalDistance);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::randomNeighbour(parameter, minimalDistance, maximalDistance), std::logic_error);
      }
    }
  }
}
