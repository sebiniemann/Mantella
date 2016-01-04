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

    arma::Mat<double>::fixed<3, 3> rollRotationMatrix({1.0, 0.0, 0.0, 0.0, std::cos(rollAngle), -std::sin(rollAngle), 0.0, std::sin(rollAngle), std::cos(rollAngle)});
    CAPTURE(rollRotationMatrix);

    arma::Mat<double>::fixed<3, 3> pitchRotationMatrix({std::cos(pitchAngle), 0.0, std::sin(pitchAngle), 0.0, 1.0, 0.0, -std::sin(pitchAngle), 0.0, std::cos(pitchAngle)});
    CAPTURE(pitchRotationMatrix);

    arma::Mat<double>::fixed<3, 3> yawRotationMatrix({std::cos(yawAngle), -std::sin(yawAngle), 0.0, std::sin(yawAngle), std::cos(yawAngle), 0.0, 0.0, 0.0, 1.0});
    CAPTURE(yawRotationMatrix);

    const arma::Mat<double>& expected = rollRotationMatrix * pitchRotationMatrix * yawRotationMatrix;
    CAPTURE(expected);

    IS_EQUAL(mant::rotationMatrix3D(rollAngle, pitchAngle, yawAngle), expected);
  }
}

SCENARIO("circleCircleIntersections") {
  GIVEN("Two circles") {
    WHEN("Their centers distance is within (*max(firstRadius, secondRadius)*, *firstRadius + secondRadius*)") {
      THEN("There are two intersections") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }

    WHEN("Their centers distance is within (*abs(firstRadius - secondRadius), *max(firstRadius, secondRadius)*)") {
      THEN("There are two intersections") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }

    WHEN("Their centers distance is (*max(firstRadius, secondRadius)*, *firstRadius + secondRadius*)") {
      THEN("There are two intersections") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }

    WHEN("Their centers distance is exactly *firstRadius + secondRadius*") {
      THEN("There is one intersection") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }

    WHEN("Their centers distance is exactly *abs(firstRadius - secondRadius)*") {
      THEN("There is one intersection") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }

    WHEN("The first circle is a dot (the radius is 0 and their distance is equal to *firstRadius*) on the second circle (which is not a dot)") {
      THEN("There is one intersection") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }

    WHEN("Both circles are dots (both radii are 0) with an identical center") {
      THEN("There is one intersection") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }

    WHEN("Their centers distance is greater than *firstRadius + secondRadius*") {
      THEN("There is no intersection") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }

    WHEN("Their centers distance is less than *abs(firstRadius - secondRadius)*") {
      THEN("There is no intersection") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }

    WHEN("Both circles are identical (same radius and center)") {
      THEN("An exception is thrown, as there are infinite intersections.") {
        IS_EQUAL(mant::circleCircleIntersections({-2.0, 3.0}, 1.3, {-1.5, 2.4}, 1.2), {{-0.74283680360129, 3.3309693303323}, {-2.5522451636118, 1.8231290303235}});
      }
    }
  }
}

SCENARIO("circleSphereIntersections") {
  GIVEN("A sphere and a circle") {
    WHEN("The sphere intersects with the circle's plane") {
      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is within (*max(circleRadius, intersectionCircleRadius)*, *circleRadius + intersectionCircleRadius*)") {
        THEN("There are two intersection") {
          IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is within (*abs(circleRadius - intersectionCircleRadius), max(circleRadius, intersectionCircleRadius)*)") {
        THEN("There are two intersection") {
          IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is exactly *circleRadius + intersectionCircleRadius*") {
        THEN("There is one intersection") {
          IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is exactly *abs(circleRadius - intersectionCircleRadius)*") {
        THEN("There is one intersection") {
          IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
        }
      }

      AND_WHEN("The sphere's intersection circle is a dot (the radius is 0 and their distance is equal to *circleRadius*) on the circle (which is not a dot)") {
        THEN("There is one intersection") {
          IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
        }
      }

      AND_WHEN("Both the sphere's intersection circle and the circle are dots (both radii are 0) with an identical center") {
        THEN("There is one intersection") {
          IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is greater than *circleRadius + intersectionCircleRadius*") {
        THEN("There is no intersection") {
          IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is less than *abs(circleRadius - intersectionCircleRadius)*") {
        THEN("There is no intersection") {
          IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
        }
      }

      AND_WHEN("The sphere's intersection circle is identical (same radius and center) to the circle") {
        THEN("An exception is thrown, as there are infinite intersections.") {
          IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
        }
      }
    }

    WHEN("The sphere does not intersect with the circle's plane") {
      THEN("There is no intersection") {
        IS_EQUAL(mant::circleSphereIntersections({0.0, -2.0, 0.0}, 1.2, {1.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, 1.5), {{0.0, -2.095, 1.1962336728}, {0.0, -2.095, -1.1962336728}});
      }
    }
  }
}
