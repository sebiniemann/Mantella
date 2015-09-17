// Catch
#include <catch.hpp>

// C++ standard library
#include <cstdlib>
#include <cmath>

// Mantella
#include <mantella>

TEST_CASE(
    "getRandomRotationMatrix") {
  SECTION(
      "Generates uniform distributed 2-dimensional rotations.") {
    arma::Col<double>::fixed<10000> angles;
    for (arma::uword n = 0; n < angles.n_elem; ++n) {
      arma::Col<double>::fixed<2> rotatedUnitVector = mant::getRandomRotationMatrix(2) * arma::normalise(arma::ones<arma::Col<double>>(2));
      angles(n) = std::atan2(rotatedUnitVector(1), rotatedUnitVector(0));
    }
    CAPTURE(angles);

    const arma::Col<arma::uword>& histogram = arma::hist(angles, 10);
    CAPTURE(histogram);
    CHECK(std::abs(histogram.max() - histogram.min()) < 0.05 * static_cast<double>(angles.n_elem));
  }

  SECTION(
      "Generates uniform distributed 3-dimensional rotations.") {
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
    CAPTURE(pitchAngles);
    CAPTURE(yawAngles);

    arma::Col<arma::uword> histogram;

    histogram = arma::hist(rollAngles, 10);
    CAPTURE(histogram);
    CHECK(std::abs(histogram.max() - histogram.min()) < 0.05 * static_cast<double>(rollAngles.n_elem));

    histogram = arma::hist(pitchAngles, 10);
    CAPTURE(histogram);
    CHECK(std::abs(histogram.max() - histogram.min()) < 0.05 * static_cast<double>(pitchAngles.n_elem));

    histogram = arma::hist(yawAngles, 10);
    CAPTURE(histogram);
    CHECK(std::abs(histogram.max() - histogram.min()) < 0.05 * static_cast<double>(yawAngles.n_elem));
  }
}

TEST_CASE(
    "getRandomPermutation") {
  SECTION(
      "Generates uniform distributed permutations.") {
    arma::Mat<arma::uword>::fixed<10, 10000> permutations;
    for (arma::uword n = 0; n < permutations.n_cols; ++n) {
      permutations.col(n) = mant::getRandomPermutation(permutations.n_rows);
    }
    CAPTURE(permutations);

    arma::Col<arma::uword> centres(permutations.n_rows);
    for (arma::uword n = 0; n < permutations.n_rows; ++n) {
      centres(n) = n;
    }

    const arma::Mat<arma::uword>& histogram = arma::hist(permutations, centres, 1);
    CAPTURE(histogram);
    CHECK(arma::all(static_cast<arma::Row<arma::uword>>(arma::abs(arma::max(histogram) - arma::min(histogram)) < 0.05 * static_cast<double>(permutations.n_cols))));
  }

  SECTION(
      "Generates uniform distributed partial permutations.") {
    arma::Mat<arma::uword>::fixed<10, 10000> permutations;
    for (arma::uword n = 0; n < permutations.n_cols; ++n) {
      permutations.col(n) = mant::getRandomPermutation(permutations.n_rows + 1, permutations.n_rows);
    }
    CAPTURE(permutations);

    arma::Col<arma::uword> centres(permutations.n_rows + 1);
    for (arma::uword n = 0; n < centres.n_elem; ++n) {
      centres(n) = n;
    }

    // arma::hist(permutations, permutations.n_rows + 1, 1) does no seem to be implemented (despite noted in Armadillo's documentation).
    const arma::Mat<arma::uword>& histogram = arma::hist(permutations, centres, 1);
    CAPTURE(histogram);
    CHECK(arma::all(static_cast<arma::Row<arma::uword>>(arma::abs(arma::max(histogram) - arma::min(histogram)) < 0.05 * static_cast<double>(permutations.n_cols))));
  }

  SECTION(
      "Throws an exception, if the cycle size is larger than the number of elements.") {
    const arma::uword numberOfElements = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfElements);

    const arma::uword cycleSize = numberOfElements + std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    CAPTURE(cycleSize);

    CHECK_THROWS_AS(mant::getRandomPermutation(numberOfElements, cycleSize), std::logic_error);
  }
}
