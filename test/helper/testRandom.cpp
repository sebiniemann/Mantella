// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <cmath>

// Mantella
#include <mantella>

TEST_CASE("random: getRandomRotationMatrix") {
  SECTION("Generates uniform distributed 2-dimensional rotation.") {
    arma::Col<double>::fixed<10000> angles;
    for (arma::uword n = 0; n < angles.n_elem; ++n) {
      arma::Col<double>::fixed<2> rotatedUnitVector = mant::getRandomRotationMatrix(2) * arma::Col<double>::fixed<2>({-2.0, 1.0});
      angles.at(n) = std::atan2(rotatedUnitVector.at(1), rotatedUnitVector.at(0));
    }

    arma::Col<arma::uword> histogram = arma::hist(angles, arma::linspace<arma::Col<double>>(-arma::datum::pi + arma::datum::pi/10.0, arma::datum::pi - arma::datum::pi/10.0, 10));
    CHECK(0.25 > static_cast<double>(histogram.max() - histogram.min()) / static_cast<double>(angles.n_elem));
  }

  SECTION("Generates uniform distributed 3-dimensional rotation.") {
    arma::Col<double>::fixed<10000> rollAngles;
    arma::Col<double>::fixed<10000> pitchAngles;
    arma::Col<double>::fixed<10000> yawAngles;
    for (arma::uword n = 0; n < rollAngles.n_elem; ++n) {
      arma::Col<double>::fixed<3> rotatedUnitVector = mant::getRandomRotationMatrix(3) * arma::Col<double>::fixed<3>({-2.0, 1.0, 3.0});
      rollAngles.at(n) = std::atan2(rotatedUnitVector.at(1), rotatedUnitVector.at(0));
      pitchAngles.at(n) = std::atan2(rotatedUnitVector.at(2), rotatedUnitVector.at(1));
      yawAngles.at(n) = std::atan2(rotatedUnitVector.at(0), rotatedUnitVector.at(2));
    }

    arma::Col<arma::uword> histogram;

    histogram = arma::hist(rollAngles, arma::linspace<arma::Col<double>>(-arma::datum::pi + arma::datum::pi/10.0, arma::datum::pi - arma::datum::pi/10.0, 10));
    CHECK(0.25 > static_cast<double>(histogram.max() - histogram.min()) / static_cast<double>(rollAngles.n_elem));

    histogram = arma::hist(pitchAngles, arma::linspace<arma::Col<double>>(-arma::datum::pi + arma::datum::pi/10.0, arma::datum::pi - arma::datum::pi/10.0, 10));
    CHECK(0.25 > static_cast<double>(histogram.max() - histogram.min()) / static_cast<double>(pitchAngles.n_elem));

    histogram = arma::hist(yawAngles, arma::linspace<arma::Col<double>>(-arma::datum::pi + arma::datum::pi/10.0, arma::datum::pi - arma::datum::pi/10.0, 10));
    CHECK(0.25 > static_cast<double>(histogram.max() - histogram.min()) / static_cast<double>(yawAngles.n_elem));
  }
}

TEST_CASE("random: getRandomPermutation") {
  SECTION("Generates uniform distributed partitial permutations.") {
    arma::Mat<arma::uword>::fixed<10, 40000> permutations;
    for (arma::uword n = 0; n < permutations.n_cols; ++n) {
      permutations.col(n) = mant::getRandomPermutation(permutations.n_rows + 1, permutations.n_rows);
    }

    arma::Col<arma::uword> centers(permutations.n_rows);
    for (arma::uword n = 0; n < permutations.n_rows; ++n) {
      centers.at(n) = n;
    }

    for (arma::uword n = 0; n < permutations.n_rows; ++n) {
      arma::Row<arma::uword> histogram = arma::hist(permutations.row(n), centers);
      CHECK(0.25 > static_cast<double>(histogram.max() - histogram.min()) / static_cast<double>(permutations.n_cols));
    }
  }

  SECTION("Throws an exception, if cycle size > number of elements.") {
    CHECK_THROWS_AS(mant::getRandomPermutation(10, 11), std::logic_error);
  }
}

TEST_CASE("random: getRandomPermutation(unsigned int)") {
  SECTION("Generates uniform distributed permutations.") {
    arma::Mat<arma::uword>::fixed<10, 10000> permutations;
    for (arma::uword n = 0; n < permutations.n_cols; ++n) {
      permutations.col(n) = mant::getRandomPermutation(permutations.n_rows);
    }

    arma::Col<arma::uword> centers(permutations.n_rows);
    for (arma::uword n = 0; n < permutations.n_rows; ++n) {
      centers.at(n) = n;
    }

    for (arma::uword n = 0; n < permutations.n_rows; ++n) {
      arma::Row<arma::uword> histogram = arma::hist(permutations.row(n), centers);
      CHECK(0.25 > static_cast<double>(histogram.max() - histogram.min()) / static_cast<double>(permutations.n_cols));
    }
  }
}

