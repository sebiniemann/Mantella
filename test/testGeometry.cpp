// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <cmath>
#include <random>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("rotationMatrix2D") {
  SECTION("Generates 2D rotation matrix.") {
    const double angle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    CAPTURE(angle);

    arma::Mat<double>::fixed<2, 2> expected({std::cos(angle), -std::sin(angle), std::sin(angle), std::cos(angle)});
    CAPTURE(expected);

    IS_EQUAL(mant::rotationMatrix2D(angle), expected);
  }
}

TEST_CASE("rotationMatrix3D") {
  SECTION("Generates 3D rotation matrix.") {
    const double rollAngle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    const double pitchAngle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    const double yawAngle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    CAPTURE(rollAngle);
    CAPTURE(pitchAngle);
    CAPTURE(yawAngle);

    arma::Mat<double>::fixed<3, 3> rollRotationMatrix({
        1.0, 0.0, 0.0, 0.0, std::cos(rollAngle), -std::sin(rollAngle), 0.0, std::sin(rollAngle), std::cos(rollAngle)
    });
    CAPTURE(rollRotationMatrix);

    arma::Mat<double>::fixed<3, 3> pitchRotationMatrix({
      std::cos(pitchAngle), 0.0, std::sin(pitchAngle), 0.0, 1.0, 0.0, -std::sin(pitchAngle), 0.0, std::cos(pitchAngle)
    });
    CAPTURE(pitchRotationMatrix);

    arma::Mat<double>::fixed<3, 3> yawRotationMatrix({
      std::cos(yawAngle), -std::sin(yawAngle), 0.0, std::sin(yawAngle), std::cos(yawAngle), 0.0, 0.0, 0.0, 1.0
    });
    CAPTURE(yawRotationMatrix);

    const arma::Mat<double>& expected = rollRotationMatrix * pitchRotationMatrix * yawRotationMatrix;
    CAPTURE(expected);

    IS_EQUAL(mant::rotationMatrix3D(rollAngle, pitchAngle, yawAngle), expected);
  }
}

TEST_CASE("circleCircleIntersections") {
  SECTION("Finds the intersection between two circles.") {
    arma::Mat<double>::fixed<2, 2> expected;

    expected = {
      -0.74283680360129, 3.3309693303323, // First intersection.
      -2.5522451636118, 1.8231290303235 // Second intersection
    };
    IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), expected);

    expected = {
      -1.7957464402521, -0.7476850918245, // First intersection.
      -0.18041433590865, 0.97533581947522 // Second intersection
    };
    IS_EQUAL(mant::circleCircleIntersections({1.8, -2.5}, 4.0, {-3.0, 2.0}, 3.0), expected);

    expected = {
      -3.2, 4.1, // First intersection
      -3.2, 4.1 // Second intersection.
    };
    IS_EQUAL(mant::circleCircleIntersections({-2.0, 5.0}, 1.5, {-6.0, 2.0}, 3.5), expected);
  }

  SECTION("Exception tests:") {
    SECTION("Throws an exception, if both centres are on the same spot.") {
      CHECK_THROWS_AS(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-2.0, 3.0}, 1.3), std::logic_error);
    }

    SECTION("Throws an exception, if the circles are to far apart.") {
      CHECK_THROWS_AS(mant::circleCircleIntersections({-8.0, 3.0}, 1.3, {-2.0, 3.0}, 1.3), std::logic_error);
    }

    SECTION("Throws an exception, if the first circle is enclosed by second one.") {
      CHECK_THROWS_AS(mant::circleCircleIntersections({-4.0, 2.8}, 0.3, {-4.0, 3.0}, 1.3), std::logic_error);
    }

    SECTION("Throws an exception, if the second circle is enclosed by first one.") {
      CHECK_THROWS_AS(mant::circleCircleIntersections({6.8, 1.4}, 2.0, {6.5, 1.2}, 0.1), std::logic_error);
    }

    SECTION("Throws an exception, if any radius is 0 or negative.") {
      CHECK_THROWS_AS(mant::circleCircleIntersections({1.8, -2.5}, 0.0, {1.8, 0.5}, 3.0), std::logic_error);
      CHECK_THROWS_AS(mant::circleCircleIntersections({1.8, -2.5}, 4.0, {-3.0, 2.0}, -3.0), std::logic_error);
    }
  }
}

TEST_CASE("circleSphereIntersections") {
  SECTION("Finds the intersection between a circle and a sphere.") {
    arma::Mat<double>::fixed<3, 2> expected;

    expected = {
      0.0, -2.095, 1.1962336728, // First intersection
      0.0, -2.095, -1.1962336728 // Second intersection
    };
    IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), expected);

    expected = {
      -2.5522451636, 1.8231290303, 0.0, // First intersection
      -0.7428368036, 3.3309693303, 0.0 // Second intersection
    };
    IS_EQUAL(mant::circleSphereIntersections({-2.0, 3.0, 0.0}, 1.3, {0.0, 0.0, 1.0}, {-1.5, 2.4, 0.0}, 1.2), expected);

    expected = {
      -0.1804143359, 0.9753358195, 0.0, // First intersection
      -1.7957464403, -0.7476850918, 0.0 // Second intersection
    };
    IS_EQUAL(mant::circleSphereIntersections({1.8, -2.5, 0.0}, 4.0, {0.0, 0.0, 1.0}, {-3.0, 2.0, 0.0}, 3.0), expected);

    expected = {
      -3.2, 4.1, 0.0, // First intersection
      -3.2, 4.1, 0.0 // Second intersection.
    };
    IS_EQUAL(mant::circleSphereIntersections({-2.0, 5.0, 0.0}, 1.5, {0.0, 0.0, 1.0}, {-6.0, 2.0, 0.0}, 3.5), expected);
  }

  SECTION("Exception tests:") {
    SECTION("Throws an exception, if both centres are on the same spot.") {
      CHECK_THROWS_AS(mant::circleSphereIntersections({-2.0, 0.0, 0.0}, 1.2, {0.0, 0.0, 1.0}, {-2.0, 0.0, 0.0}, 1.5), std::logic_error);
    }

    SECTION("Throws an exception, if the circle and sphere are to far apart.") {
      CHECK_THROWS_AS(mant::circleSphereIntersections({-7.0, 3.0, 0.0}, 1.3, {0.0, 0.0, 1.0}, {-1.5, 2.4, 0.0}, 1.2), std::logic_error);
    }

    SECTION("Throws an exception, if the circle is enclosed by the sphere.") {
      CHECK_THROWS_AS(mant::circleSphereIntersections({-2.0, 3.0, 0.0}, 1.3, {0.0, 1.0, 1.0}, {-1.5, 2.4, 0.0}, 5.0), std::logic_error);
    }

    SECTION("Throws an exception, if the sphere is enclosed by the circle.") {
      CHECK_THROWS_AS(mant::circleSphereIntersections({-2.0, 3.0, 0.0}, 6.3, {0.0, 1.0, 1.0}, {-1.5, 2.4, 0.0}, 0.2), std::logic_error);
    }

    SECTION("Throws an exception, if any radius is 0 or negative.") {
      CHECK_THROWS_AS(mant::circleSphereIntersections({-1.5, 3.4, 0.0}, 1.0, {0.0, 0.0, 1.0}, {-1.5, 2.4, 0.0}, 0.0), std::logic_error);
      CHECK_THROWS_AS(mant::circleSphereIntersections({1.8, -2.5, 0.0}, -4.0, {0.0, 0.0, 1.0}, {-3.0, 2.0, 0.0}, 3.0), std::logic_error);
    }
  }
}
