// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <cmath>

// Mantella
#include <mantella>

TEST_CASE("getRandomRotationMatrix") {
  SECTION("Generates uniform distributed 2-dimensional rotations.") {
    arma::Col<double>::fixed<10000> angles;
    for (arma::uword n = 0; n < angles.n_elem; ++n) {
      arma::Col<double>::fixed<2> rotatedUnitVector = mant::getRandomRotationMatrix(2) * arma::normalise(arma::ones<arma::Col<double>>(2));
      angles(n) = std::atan2(rotatedUnitVector(1), rotatedUnitVector(0));
    }
    CAPTURE(angles);

    IS_UNIFORM(angles, -arma::datum::pi, arma::datum::pi);
  }

  SECTION("Generates uniform distributed 3-dimensional rotations.") {
    arma::Col<double>::fixed<10000> rollAngles;
    arma::Col<double>::fixed<10000> pitchAngles;
    arma::Col<double>::fixed<10000> yawAngles;
    for (arma::uword n = 0; n < rollAngles.n_elem; ++n) {
      arma::Col<double>::fixed<3> rotatedUnitVector = mant::getRandomRotationMatrix(3) * arma::normalise(arma::ones<arma::Col<double>>(3));
      rollAngles(n) = std::atan2(rotatedUnitVector(1), rotatedUnitVector(0));
      pitchAngles(n) = std::atan2(rotatedUnitVector(2), rotatedUnitVector(1));
      yawAngles(n) = std::atan2(rotatedUnitVector(0), rotatedUnitVector(2));
    }

    CAPTURE(rollAngles);
    IS_UNIFORM(rollAngles, -arma::datum::pi, arma::datum::pi);
    
    CAPTURE(pitchAngles);
    IS_UNIFORM(pitchAngles, -arma::datum::pi, arma::datum::pi);
    
    CAPTURE(yawAngles);
    IS_UNIFORM(yawAngles, -arma::datum::pi, arma::datum::pi);
  }
}

TEST_CASE("getRandomPermutation") {
  SECTION("Generates uniform distributed permutations.") {
    arma::Mat<arma::uword>::fixed<10000, 10> permutations;
    for (arma::uword n = 0; n < permutations.n_rows; ++n) {
      permutations.row(n) = mant::getRandomPermutation(permutations.n_cols).t();
    }
    
    for (arma::uword n = 0; n < permutations.n_cols; ++n) {
      CAPTURE(permutations.col(n));
      IS_UNIFORM(permutations.col(n), 0, permutations.n_cols - 1);
    }
  }

  SECTION("Generates uniform distributed partial permutations.") {
    arma::Mat<arma::uword>::fixed<10000, 10> permutations;
    for (arma::uword n = 0; n < permutations.n_rows; ++n) {
      permutations.row(n) = mant::getRandomPermutation(permutations.n_cols + 1, permutations.n_cols).t();
    }
    
    for (arma::uword n = 0; n < permutations.n_cols; ++n) {
      CAPTURE(permutations.col(n));
      IS_UNIFORM(permutations.col(n), 0, permutations.n_cols);
    }
  }

  SECTION("Throws an exception, if the cycle size is larger than the number of elements.") {
    const arma::uword numberOfElements = getRandomNumberOfValues();
    CAPTURE(numberOfElements);

    const arma::uword cycleSize = getRandomNumberOfValues(numberOfElements + 1);
    CAPTURE(cycleSize);

    CHECK_THROWS_AS(mant::getRandomPermutation(numberOfElements, cycleSize), std::logic_error);
  }
}
