// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cmath>
#include <random>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE(
    "get2DRotation") {
  SECTION(
      "Generates 2D rotation matrix.") {
    const double angle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    CAPTURE(angle);

    arma::Mat<double>::fixed<2, 2> expected({std::cos(angle), -std::sin(angle), std::sin(angle), std::cos(angle)});
    CAPTURE(expected);

    COMPARE(mant::get2DRotation(angle), expected);
  }
}

TEST_CASE(
    "get3DRotation") {
  SECTION(
      "Generates 3D rotation matrix.") {
    const double rollAngle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    const double pitchAngle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    const double yawAngle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    CAPTURE(rollAngle);
    CAPTURE(pitchAngle);
    CAPTURE(yawAngle);

    arma::Mat<double>::fixed<3, 3> rollRotationMatrix({
        1.0, 0.0, 0.0, 0.0, std::cos(rollAngle), -std::sin(rollAngle), 0.0, std::sin(rollAngle), std::cos(rollAngle),
    });
    CAPTURE(rollRotationMatrix);

    arma::Mat<double>::fixed<3, 3> pitchRotationMatrix({std::cos(pitchAngle), 0.0, std::sin(pitchAngle), 0.0, 1.0, 0.0, -std::sin(pitchAngle), 0.0, std::cos(pitchAngle)});
    CAPTURE(pitchRotationMatrix);

    arma::Mat<double>::fixed<3, 3> yawRotationMatrix({std::cos(yawAngle), -std::sin(yawAngle), 0.0, std::sin(yawAngle), std::cos(yawAngle), 0.0, 0.0, 0.0, 1.0});
    CAPTURE(yawRotationMatrix);

    const arma::Mat<double>& expected = rollRotationMatrix * pitchRotationMatrix * yawRotationMatrix;
    CAPTURE(expected);

    COMPARE(mant::get3DRotation(rollAngle, pitchAngle, yawAngle), expected);
  }
}

TEST_CASE(
    "getCircleCircleIntersection") {
  SECTION(
      "Finds the intersection between two circles.") {
    arma::Col<double>::fixed<2> expected;

    expected = {-2.5522451636, 1.8231290303};
    COMPARE(mant::getCircleCircleIntersection({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), expected);

    expected = {-0.1804143359, 0.9753358195};
    COMPARE(mant::getCircleCircleIntersection({1.8, -2.5}, 4.0, {-3.0, 2.0}, 3.0), expected);
  }

  SECTION(
      "Exception tests") {
    SECTION(
        "Throws an exception, if both centers are on the same spot.") {
      CHECK_THROWS_AS(mant::getCircleCircleIntersection({-2.0, 3.0}, 1.3, {-2.0, 3.0}, 1.3), std::logic_error);
    }

    SECTION(
        "Throws an exception, if the circles are to far apart.") {
      CHECK_THROWS_AS(mant::getCircleCircleIntersection({-8.0, 3.0}, 1.3, {-2.0, 3.0}, 1.3), std::logic_error);
    }

    SECTION(
        "Throws an exception, if the first circle is enclosed by second one.") {
      CHECK_THROWS_AS(mant::getCircleCircleIntersection({-4.0, 2.8}, 0.3, {-4.0, 3.0}, 1.3), std::logic_error);
    }

    SECTION(
        "Throws an exception, if the second circle is enclosed by first one.") {
      CHECK_THROWS_AS(mant::getCircleCircleIntersection({6.8, 1.4}, 2.0, {6.5, 1.2}, 0.1), std::logic_error);
    }

    SECTION(
        "Throws an exception, if any radius is 0 or negative.") {
      CHECK_THROWS_AS(mant::getCircleCircleIntersection({1.8, -2.5}, 0.0, {1.8, 0.5}, 3.0), std::logic_error);
      CHECK_THROWS_AS(mant::getCircleCircleIntersection({1.8, -2.5}, 4.0, {-3.0, 2.0}, -3.0), std::logic_error);
    }
  }
}

TEST_CASE(
    "getCircleSphereIntersection") {
  SECTION(
      "Finds the intersection between a circle and a sphere.") {
    arma::Col<double>::fixed<3> expected;

    expected = {-2.095, 1.1962336728, 0.0};
    COMPARE(mant::getCircleSphereIntersection({-2.0, 0.0, 0.0}, 1.2, {0.0, 0.0, 1.0}, {-3.0, 0.0, 0.0}, 1.5), expected);

    expected = {-2.5522451636, 1.8231290303, 0.0};
    COMPARE(mant::getCircleSphereIntersection({-2.0, 3.0, 0.0}, 1.3, {0.0, 0.0, 1.0}, {-1.5, 2.4, 0.0}, 1.2), expected);

    expected = {-0.1804143359, 0.9753358195, 0.0};
    COMPARE(mant::getCircleSphereIntersection({1.8, -2.5, 0.0}, 4.0, {0.0, 0.0, 1.0}, {-3.0, 2.0, 0.0}, 3.0), expected);
  }

  SECTION(
      "Exception tests") {
    SECTION(
        "Throws an exception, if both centers are on the same spot.") {
      CHECK_THROWS_AS(mant::getCircleSphereIntersection({-2.0, 0.0, 0.0}, 1.2, {0.0, 0.0, 1.0}, {-2.0, 0.0, 0.0}, 1.5), std::logic_error);
    }

    SECTION(
        "Throws an exception, if the circle and sphere are to far apart.") {
      CHECK_THROWS_AS(mant::getCircleSphereIntersection({-7.0, 3.0, 0.0}, 1.3, {0.0, 0.0, 1.0}, {-1.5, 2.4, 0.0}, 1.2), std::logic_error);
    }

    SECTION(
        "Throws an exception, if the circle is enclosed by the sphere.") {
      CHECK_THROWS_AS(mant::getCircleSphereIntersection({-2.0, 3.0, 0.0}, 1.3, {0.0, 1.0, 1.0}, {-1.5, 2.4, 0.0}, 5.0), std::logic_error);
    }

    SECTION(
        "Throws an exception, if the sphere is enclosed by the circle.") {
      CHECK_THROWS_AS(mant::getCircleSphereIntersection({-2.0, 3.0, 0.0}, 6.3, {0.0, 1.0, 1.0}, {-1.5, 2.4, 0.0}, 0.2), std::logic_error);
    }

    SECTION(
        "Throws an exception, if any radius is 0 or negative.") {
      CHECK_THROWS_AS(mant::getCircleSphereIntersection({-1.5, 3.4, 0.0}, 1.0, {0.0, 0.0, 1.0}, {-1.5, 2.4, 0.0}, 0.0), std::logic_error);
      CHECK_THROWS_AS(mant::getCircleSphereIntersection({1.8, -2.5, 0.0}, -4.0, {0.0, 0.0, 1.0}, {-3.0, 2.0, 0.0}, 3.0), std::logic_error);
    }
  }
}
