// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <cmath>
#include <array>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("Circle circle intersection", "") {
  arma::Col<double>::fixed<2> result;
  arma::Col<double>::fixed<2> expected;

  result = mant::getCircleCircleIntersection({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2);
  expected = {-2.5522451636, 1.8231290303};
  for (std::size_t n = 0; n < expected.n_elem; ++n) {
    CHECK(result.at(n) == Approx(expected.at(n)));
  }

  result = mant::getCircleCircleIntersection({1.8, -2.5}, 4.0, {-3.0, 2.0}, 3.0);
  expected = {-0.1804143359, 0.9753358195};
  for (std::size_t n = 0; n < expected.n_elem; ++n) {
    CHECK(result.at(n) == Approx(expected.at(n)));
  }
}

TEST_CASE("Circle sphere intersection", "") {
  arma::Col<double>::fixed<3> result;
  arma::Col<double>::fixed<3> expected;

  result = mant::getCircleSphereIntersection({-2.0, 0.0, 0.0}, 1.2, {0.0, 0.0, 1.0}, {-3.0, 0.0, 0.0}, 1.5);
  expected = {-2.095, 1.1962336728, 0.0};
  for (std::size_t n = 0; n < expected.n_elem; ++n) {
    CHECK(result.at(n) == Approx(expected.at(n)));
  }

  result = mant::getCircleSphereIntersection({-2.0, 3.0, 0.0}, 1.3, {0.0, 0.0, 1.0}, {-1.5, 2.4, 0.0}, 1.2);
  expected = {-2.5522451636, 1.8231290303, 0.0};
  for (std::size_t n = 0; n < expected.n_elem; ++n) {
    CHECK(result.at(n) == Approx(expected.at(n)));
  }

  result = mant::getCircleSphereIntersection({1.8, -2.5, 0.0}, 4.0, {0.0, 0.0, 1.0}, {-3.0, 2.0, 0.0}, 3.0);
  expected = {-0.1804143359, 0.9753358195, 0.0};
  for (std::size_t n = 0; n < expected.n_elem; ++n) {
    CHECK(result.at(n) == Approx(expected.at(n)));
  }
}

TEST_CASE("2D rotation matrix", "") {
  std::array<double, 15> angles = {{0.0,  45.0, 90.0, 135.0, 180.0, 225.0, 270.0, 315.0, 360.0, -0.0, -45.0, -90.0, -180.0, -225.0, -315.0}};

  for (const auto& angle : angles) {
    arma::Mat<double> result = mant::get2DRotationMatrix(angle);

    arma::Mat<double>::fixed<2, 2> expected({
      std::cos(angle), -std::sin(angle),
      std::sin(angle), std::cos(angle)
    });

    CHECK(result.n_rows == 2);
    CHECK(result.n_cols == 2);

    for (std::size_t n = 0; n < expected.n_elem; ++n) {
      CHECK(result.at(n) == Approx(expected.at(n)));
    }
  }
}

TEST_CASE("3D rotation matrix", "") {
  std::array<double, 14> rollAngles = {{0.0, 45.0, 90.0, 135.0, 180.0, 225, 270, 315, 360, -0, -45, 276, -56, -45.89}};
  std::array<double, 14> pitchAngles = {{0.0, 45.0, 90.0, 135.0, 180.0, 225, 270, 315, 360, -0, -90, -89, 78, -245}};
  std::array<double, 14> yawAngles = {{0.0, 45.0, 90.0, 135.0, 180.0, 225, 270, 315, 360, -0, -225, -310, -90, 345}};

  for (const auto& rollAngle : rollAngles) {
    for (const auto& pitchAngle : pitchAngles) {
      for (const auto& yawAngle : yawAngles) {
        arma::Mat<double> result = mant::get3DRotationMatrix(rollAngle, pitchAngle, yawAngle);

        arma::Mat<double>::fixed<3, 3> expectedRoll({
          1.0, 0.0, 0.0,
          0.0, std::cos(rollAngle), -std::sin(rollAngle),
          0.0, std::sin(rollAngle), std::cos(rollAngle),
        });

        arma::Mat<double>::fixed<3, 3> expectedPitch({
          std::cos(pitchAngle), 0.0, std::sin(pitchAngle),
          0.0, 1.0, 0.0,
          -std::sin(pitchAngle), 0.0, std::cos(pitchAngle)
        });

        arma::Mat<double>::fixed<3, 3> expectedYaw({
          std::cos(yawAngle), -std::sin(yawAngle), 0.0,
          std::sin(yawAngle), std::cos(yawAngle), 0.0,
          0.0, 0.0, 1.0
        });

        arma::Mat<double>::fixed<3, 3> expected = expectedRoll * expectedPitch * expectedYaw;

        CHECK(result.n_rows == 3);
        CHECK(result.n_cols == 3);

        for (std::size_t n = 0; n < expected.n_elem; ++n) {
          CHECK(result.at(n) == Approx(expected.at(n)));
        }
      }
    }
  }
}

