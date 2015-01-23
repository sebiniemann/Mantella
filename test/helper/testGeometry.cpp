// Catch
#include <catch.hpp>
#include <helper.hpp>

// C++ Standard Library
#include <cmath>
#include <array>

// Mantella
#include <mantella>

TEST_CASE("geometry: get2DRotationMatrix(...)", "") {
  SECTION("Generates 2D rotation matrix") {
    const std::array<double, 15>& angles = {{0.0,  45.0, 90.0, 135.0, 180.0, 225.0, 270.0, 315.0, 360.0, -0.0, -45.0, -90.0, -180.0, -225.0, -315.0}};

    for (const auto& angle : angles) {
      arma::Mat<double>::fixed<2, 2> expected({
        std::cos(angle), -std::sin(angle),
        std::sin(angle), std::cos(angle)
      });

      compare(mant::get2DRotationMatrix(angle), expected);
    }
  }
}

TEST_CASE("geometry: get3DRotationMatrix(...)", "") {
  SECTION("Generates 3D rotation matrix") {
    const std::array<double, 14>& rollAngles = {{0.0, 45.0, 90.0, 135.0, 180.0, 225, 270, 315, 360, -0, -45, 276, -56, -45.89}};
    const std::array<double, 14>& pitchAngles = {{0.0, 45.0, 90.0, 135.0, 180.0, 225, 270, 315, 360, -0, -90, -89, 78, -245}};
    const std::array<double, 14>& yawAngles = {{0.0, 45.0, 90.0, 135.0, 180.0, 225, 270, 315, 360, -0, -225, -310, -90, 345}};

    for (const auto& rollAngle : rollAngles) {
      for (const auto& pitchAngle : pitchAngles) {
        for (const auto& yawAngle : yawAngles) {
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

          compare<double>(mant::get3DRotationMatrix(rollAngle, pitchAngle, yawAngle), expectedRoll * expectedPitch * expectedYaw);
        }
      }
    }
  }
}

TEST_CASE("geometry: getCircleCircleIntersection(...)", "") {
  SECTION("Finds intersection") {
    arma::Col<double>::fixed<2> expected;

    expected = {-2.5522451636, 1.8231290303};
    compare(mant::getCircleCircleIntersection({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), expected);

    expected = {-0.1804143359, 0.9753358195};
    compare(mant::getCircleCircleIntersection({1.8, -2.5}, 4.0, {-3.0, 2.0}, 3.0), expected);
  }

  SECTION("Throws an exception, if both centers are on the same spot") {
    CHECK_THROWS_AS(mant::getCircleCircleIntersection({-2.0, 3.0}, 1.3, {-2.0, 3.0}, 1.3), std::logic_error);
  }

  SECTION("Throws an exception, if the circles are to far apart") {
    CHECK_THROWS_AS(mant::getCircleCircleIntersection({-8.0, 3.0}, 1.3, {-2.0, 3.0}, 1.3), std::logic_error);
  }

  SECTION("Throws an exception, if a circle is enclosed by the other one") {
    CHECK_THROWS_AS(mant::getCircleCircleIntersection({-2.0, 2.8}, 0.3, {-2.0, 3.0}, 1.3), std::logic_error);
  }
}

TEST_CASE("geometry: getCircleSphereIntersection(...)", "") {
  SECTION("Finds intersection") {
    arma::Col<double>::fixed<3> expected;

    expected = {-2.095, 1.1962336728, 0.0};
    compare(mant::getCircleSphereIntersection({-2.0, 0.0, 0.0}, 1.2, {0.0, 0.0, 1.0}, {-3.0, 0.0, 0.0}, 1.5), expected);

    expected = {-2.5522451636, 1.8231290303, 0.0};
    compare(mant::getCircleSphereIntersection({-2.0, 3.0, 0.0}, 1.3, {0.0, 0.0, 1.0}, {-1.5, 2.4, 0.0}, 1.2), expected);

    expected = {-0.1804143359, 0.9753358195, 0.0};
    compare(mant::getCircleSphereIntersection({1.8, -2.5, 0.0}, 4.0, {0.0, 0.0, 1.0}, {-3.0, 2.0, 0.0}, 3.0), expected);
  }
}

