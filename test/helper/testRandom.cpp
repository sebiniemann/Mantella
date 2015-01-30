// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <cmath>

// Mantella
#include <mantella>

TEST_CASE("random: getRandomRotationMatrix", "") {
  SECTION("Generates uniform distributed 2-dimensional rotation.") {
    arma::Col<double>::fixed<10000> angles;
    for (std::size_t n = 0; n < angles.n_elem; ++n) {
      arma::Col<double>::fixed<2> rotatedUnitVector = mant::getRandomRotationMatrix(2) * arma::Col<double>::fixed<2>({-2, 1});
      angles.at(n) = std::atan2(rotatedUnitVector.at(1), rotatedUnitVector.at(0));
    }

    arma::Col<unsigned int> histogram = arma::histc(angles, arma::linspace<arma::Col<double>>(-arma::datum::pi, arma::datum::pi, 10));
    CHECK(0.05 > static_cast<double>(histogram.max() - histogram.min()) / angles.n_elem);
  }

  SECTION("Generates uniform distributed 3-dimensional rotation.") {
    arma::Col<double>::fixed<10000> rollAngles;
    arma::Col<double>::fixed<10000> pitchAngles;
    arma::Col<double>::fixed<10000> yawAngles;
    for (std::size_t n = 0; n < rollAngles.n_elem; ++n) {
      arma::Col<double>::fixed<3> rotatedUnitVector = mant::getRandomRotationMatrix(3) * arma::Col<double>::fixed<3>({-2, 1, 3});
      rollAngles.at(n) = std::atan2(rotatedUnitVector.at(1), rotatedUnitVector.at(0));
      pitchAngles.at(n) = std::atan2(rotatedUnitVector.at(2), rotatedUnitVector.at(1));
      yawAngles.at(n) = std::atan2(rotatedUnitVector.at(0), rotatedUnitVector.at(2));
    }

    arma::Col<unsigned int> histogram;

    histogram = arma::histc(rollAngles, arma::linspace<arma::Col<double>>(-arma::datum::pi, arma::datum::pi, 10));
    CHECK(0.02 > static_cast<double>(histogram.max() - histogram.min()) / rollAngles.n_elem);

    histogram = arma::histc(pitchAngles, arma::linspace<arma::Col<double>>(-arma::datum::pi, arma::datum::pi, 10));
    CHECK(0.02 > static_cast<double>(histogram.max() - histogram.min()) / pitchAngles.n_elem);

    histogram = arma::histc(yawAngles, arma::linspace<arma::Col<double>>(-arma::datum::pi, arma::datum::pi, 10));
    CHECK(0.02 > static_cast<double>(histogram.max() - histogram.min()) / yawAngles.n_elem);
  }
}

TEST_CASE("random: getRandomPermutation(unsigned int, unsigned int)", "") {
  SECTION("Generates uniform distributed partitial permutations.") {
    arma::Mat<unsigned int>::fixed<10, 40000> permutations;
    for (std::size_t n = 0; n < permutations.n_cols; ++n) {
      permutations.col(n) = mant::getRandomPermutation(static_cast<unsigned int>(permutations.n_rows) + 1, static_cast<unsigned int>(permutations.n_rows));
    }

    arma::Col<unsigned int> centers(permutations.n_rows);
    for (std::size_t n = 0; n < permutations.n_rows; ++n) {
      centers.at(n) = n;
    }

    for (std::size_t n = 0; n < permutations.n_rows; ++n) {
      arma::Row<unsigned int> histogram = arma::hist(permutations.row(n), centers);
      CHECK(0.1 > static_cast<double>(histogram.max() - histogram.min()) / permutations.n_cols);
    }
  }

  SECTION("Throws an exception, if cycle size > number of elements.") {
    CHECK_THROWS_AS(mant::getRandomPermutation(10, 11), std::logic_error);
  }
}

TEST_CASE("random: getRandomPermutation(unsigned int)", "") {
  SECTION("Generates uniform distributed permutations.") {
    arma::Mat<unsigned int>::fixed<10, 10000> permutations;
    for (std::size_t n = 0; n < permutations.n_cols; ++n) {
      permutations.col(n) = mant::getRandomPermutation(static_cast<unsigned int>(permutations.n_rows));
    }

    arma::Col<unsigned int> centers(permutations.n_rows);
    for (std::size_t n = 0; n < permutations.n_rows; ++n) {
      centers.at(n) = n;
    }

    for (std::size_t n = 0; n < permutations.n_rows; ++n) {
      arma::Row<unsigned int> histogram = arma::hist(permutations.row(n), centers);
      CHECK(0.05 > static_cast<double>(histogram.max() - histogram.min()) / permutations.n_cols);
    }
  }
}

