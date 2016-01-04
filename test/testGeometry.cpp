// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("rotationMatrix2D", "[geometry][rotationMatrix2D]") {
  GIVEN("An angle") {
    const double angle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    CAPTURE(angle);

    THEN("Return a 2-dimensional rotation matrix") {
      IS_EQUAL(mant::rotationMatrix2D(angle), arma::Mat<double>::fixed<2, 2>({std::cos(angle), -std::sin(angle), std::sin(angle), std::cos(angle)}));
    }
  }
}

SCENARIO("rotationMatrix3D", "[geometry][rotationMatrix3D]") {
  GIVEN("A roll, pitch and yaw angle") {
    const double rollAngle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    CAPTURE(rollAngle);

    const double pitchAngle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    CAPTURE(pitchAngle);

    const double yawAngle = std::uniform_real_distribution<double>(-720.0, 720.0)(mant::Rng::getGenerator());
    CAPTURE(yawAngle);

    THEN("Return a 3-dimensional rotation matrix") {
      IS_EQUAL(mant::rotationMatrix3D(rollAngle, pitchAngle, yawAngle), arma::Mat<double>::fixed<3, 3>({1.0, 0.0, 0.0, 0.0, std::cos(rollAngle), -std::sin(rollAngle), 0.0, std::sin(rollAngle), std::cos(rollAngle)}) * arma::Mat<double>::fixed<3, 3>({std::cos(pitchAngle), 0.0, std::sin(pitchAngle), 0.0, 1.0, 0.0, -std::sin(pitchAngle), 0.0, std::cos(pitchAngle)}) * arma::Mat<double>::fixed<3, 3>({std::cos(yawAngle), -std::sin(yawAngle), 0.0, std::sin(yawAngle), std::cos(yawAngle), 0.0, 0.0, 0.0, 1.0}));
    }
  }
}

SCENARIO("circleCircleIntersections", "[geometry][circleCircleIntersections]") {
  GIVEN("Two circles") {
    WHEN("The centers distance is within (*max(firstRadius, secondRadius)*, *firstRadius + secondRadius*)") {
      const arma::Col<double>::fixed<2>& firstCenter = {-1.2, -2.4};
      CAPTURE(firstCenter);

      const double firstRadius = 3.2;
      CAPTURE(firstRadius);

      const arma::Col<double>::fixed<2>& secondCenter = {2.2, 1.4};
      CAPTURE(secondCenter);

      const double secondRadius = 4.1;
      CAPTURE(secondRadius);

      THEN("Return two intersections") {
        IS_EQUAL(mant::circleCircleIntersections(firstCenter, firstRadius, secondCenter, secondRadius), {{-1.8455795693519, 0.73420277257798}, {1.986425723198, -2.6944335418087}});
      }
    }

    WHEN("The centers distance is within (*abs(firstRadius - secondRadius), *max(firstRadius, secondRadius)*)") {
      const arma::Col<double>::fixed<2>& firstCenter = {-1.2, -2.4};
      CAPTURE(firstCenter);

      const double firstRadius = 3.2;
      CAPTURE(firstRadius);

      const arma::Col<double>::fixed<2>& secondCenter = {-0.1, 0.2};
      CAPTURE(secondCenter);

      const double secondRadius = 1.1;
      CAPTURE(secondRadius);

      THEN("Return two intersections") {
        IS_EQUAL(mant::circleCircleIntersections(firstCenter, firstRadius, secondCenter, secondRadius), {{-1.0250514584014, 0.79521407855444}, {0.97135007822576, -0.049417340787819}});
      }
    }

    WHEN("The centers distance is exactly *firstRadius + secondRadius*") {
      const arma::Col<double>::fixed<2>& firstCenter = {2.5, 1.2};
      CAPTURE(firstCenter);

      const double firstRadius = 3.2;
      CAPTURE(firstRadius);

      const arma::Col<double>::fixed<2>& secondCenter = {-1.3, 1.2};
      CAPTURE(secondCenter);

      const double secondRadius = 0.6;
      CAPTURE(secondRadius);

      THEN("Return 1 intersection") {
        IS_EQUAL(mant::circleCircleIntersections(firstCenter, firstRadius, secondCenter, secondRadius), {{-0.7, 1.2}});
      }
    }

    WHEN("The centers distance is exactly *abs(firstRadius - secondRadius)*") {
      const arma::Col<double>::fixed<2>& firstCenter = {2.5, 1.2};
      CAPTURE(firstCenter);

      const double firstRadius = 3.2;
      CAPTURE(firstRadius);

      const arma::Col<double>::fixed<2>& secondCenter = {5.2, 1.2};
      CAPTURE(secondCenter);

      const double secondRadius = 0.5;
      CAPTURE(secondRadius);

      THEN("Return 1 intersection") {
        IS_EQUAL(mant::circleCircleIntersections(firstCenter, firstRadius, secondCenter, secondRadius), {{5.7, 1.2}});
      }
    }

    WHEN("One circle is a dot (the radius is 0 and The distance is equal to *firstRadius*) on the other circle (which is not a dot)") {
      const arma::Col<double>::fixed<2>& firstCenter = {2.5, 1.2};
      CAPTURE(firstCenter);

      const double firstRadius = 3.2;
      CAPTURE(firstRadius);

      const arma::Col<double>::fixed<2>& secondCenter = {5.7, 1.2};
      CAPTURE(secondCenter);
      const double secondRadius = 0.0;
      CAPTURE(secondRadius);

      THEN("Return 1 intersection") {
        IS_EQUAL(mant::circleCircleIntersections(firstCenter, firstRadius, secondCenter, secondRadius), {{5.7, 1.2}});
        IS_EQUAL(mant::circleCircleIntersections(secondCenter, secondRadius, firstCenter, firstRadius), {{5.7, 1.2}});
      }
    }

    WHEN("Both circles are dots (both radii are 0) with an identical center") {
      const arma::Col<double>::fixed<2>& firstCenter = {0.1, 6.36};
      CAPTURE(firstCenter);

      const double firstRadius = 0.0;
      CAPTURE(firstRadius);

      const arma::Col<double>::fixed<2>& secondCenter = {0.1, 6.36};
      CAPTURE(secondCenter);

      const double secondRadius = 0.0;
      CAPTURE(secondRadius);

      THEN("Return 1 intersection") {
        IS_EQUAL(mant::circleCircleIntersections(firstCenter, firstRadius, secondCenter, secondRadius), {{0.1, 6.36}});
      }
    }

    WHEN("The centers distance is greater than *firstRadius + secondRadius*") {
      const arma::Col<double>::fixed<2>& firstCenter = {2.5, 1.2};
      CAPTURE(firstCenter);

      const double firstRadius = 3.2;
      CAPTURE(firstRadius);

      const arma::Col<double>::fixed<2>& secondCenter = {20.5, 10.2};
      CAPTURE(secondCenter);

      const double secondRadius = 1.2;
      CAPTURE(secondRadius);

      THEN("Return zero intersections") {
        IS_EQUAL(mant::circleCircleIntersections(firstCenter, firstRadius, secondCenter, secondRadius), {});
      }
    }

    WHEN("The centers distance is less than *abs(firstRadius - secondRadius)*") {
      const arma::Col<double>::fixed<2>& firstCenter = {2.5, 1.2};
      CAPTURE(firstCenter);

      const double firstRadius = 3.2;
      CAPTURE(firstRadius);

      const arma::Col<double>::fixed<2>& secondCenter = {2.8, 1.6};
      CAPTURE(secondCenter);

      const double secondRadius = 0.6;
      CAPTURE(secondRadius);

      THEN("Return zero intersections") {
        IS_EQUAL(mant::circleCircleIntersections(firstCenter, firstRadius, secondCenter, secondRadius), {});
      }
    }

    WHEN("Both circles are identical (same radius and center)") {
      const arma::Col<double>::fixed<2>& firstCenter = {2.5, 1.2};
      CAPTURE(firstCenter);

      const double firstRadius = 3.2;
      CAPTURE(firstRadius);

      const arma::Col<double>::fixed<2>& secondCenter = {2.5, 1.2};
      CAPTURE(secondCenter);

      const double secondRadius = 3.2;
      CAPTURE(secondRadius);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::circleCircleIntersections(firstCenter, firstRadius, secondCenter, secondRadius), std::logic_error);
      }
    }
  }
}

SCENARIO("circleSphereIntersections", "[geometry][circleSphereIntersections]") {
  GIVEN("A sphere and a circle") {
    WHEN("The sphere intersects with the circle's plane") {
      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is within (*max(circleRadius, intersectionCircleRadius)*, *circleRadius + intersectionCircleRadius*)") {
        const arma::Col<double>::fixed<3>& circleCenter = {1.71037554404510, 3.64627141711538, 7.78840646877250};
        CAPTURE(circleCenter);

        const double circleRadius = 8.08132872957838;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.285511339171737, 0.263199560882686, -0.921525510420370};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-3.44040300957287, -4.17124887185550, 0.875266289505627};
        CAPTURE(sphereCenter);

        const double sphereRadius = 6.46975191235126;
        CAPTURE(sphereRadius);

        THEN("Return two intersection") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {{1.16469480073837, -4.06037146073060, 5.41822459489890}, {-5.18238295082404, 0.666644704628342, 4.80184004206240}});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is within (*abs(circleRadius - intersectionCircleRadius), max(circleRadius, intersectionCircleRadius)*)") {
        const arma::Col<double>::fixed<3>& circleCenter = {-3.50130736516486, 1.00721993762508, 4.45910241904761};
        CAPTURE(circleCenter);

        const double circleRadius = 5.99010703040300;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.176604220410152, -0.247642470535383, -0.952619628246476};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-4.26292172088363, 1.80075110090070, 1.42342000000000};
        CAPTURE(sphereCenter);

        const double sphereRadius = 7.78421349144159;
        CAPTURE(sphereRadius);

        THEN("Return two intersection") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {{-4.91060527034701, -4.68576560570548, 5.67778090559027}, {2.39261228050498, 1.42557676033646, 5.44300841809502}});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is exactly *circleRadius + intersectionCircleRadius*") {
        const arma::Col<double>::fixed<3>& circleCenter = {2.63021058406213, -6.59774155884104, 7.57279366864463};
        CAPTURE(circleCenter);

        const double circleRadius = 4.868193677507644;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.176604220410152, -0.247642470535383, -0.952619628246476};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-4.26292172088363, 1.80075110090070, 1.42342000000000};
        CAPTURE(sphereCenter);

        const double sphereRadius = 7.78421349144159;
        CAPTURE(sphereRadius);

        THEN("Return one intersection") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {{-0.296249423553824, -2.99905684451752, 6.09475110077178}});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is exactly *abs(circleRadius - intersectionCircleRadius)*") {
        const arma::Col<double>::fixed<3>& circleCenter = {-2.17603910288870, -1.50364082820138, 5.35751318672055};
        CAPTURE(circleCenter);

        const double circleRadius = 2.58770224454292;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.090410660062076, -0.126777939782578, -0.987802240598605};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-4.26292172088363, 1.80075110090070, 1.42342000000000};
        CAPTURE(sphereCenter);

        const double sphereRadius = 7.78421349144159;
        CAPTURE(sphereRadius);

        THEN("Return one intersection") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {{-0.797087405472182, -3.65601747058010, 5.75996804375059}});
        }
      }

      AND_WHEN("The sphere's intersection circle is a dot (the radius is 0) on the circle (which is not a dot)") {
        const arma::Col<double>::fixed<3>& circleCenter = {-3.89883315146586, 4.61891105015065, 7.97098605210794};
        CAPTURE(circleCenter);

        const double circleRadius = 12.7117514791544;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.586302739220448, -0.563800193419407, 0.581703051292357};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-2.28880000000000, 1.88432000000000, -5.43603734569035};
        CAPTURE(sphereCenter);

        const double sphereRadius = 5.31317660930921;
        CAPTURE(sphereRadius);

        THEN("Return one intersection") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {{0.826330000000000, -1.11125000000000, -2.34534630000000}});
        }
      }

      AND_WHEN("The circle is a dot (the radius is 0) on the sphere (which is not a dot)") {
        const arma::Col<double>::fixed<3>& circleCenter = {-4.91060527034701, -4.68576560570548, 5.67778090559027};
        CAPTURE(circleCenter);

        const double circleRadius = 0.0;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.176604220410152, -0.247642470535383, -0.952619628246476};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-4.26292172088363, 1.80075110090070, 1.42342000000000};
        CAPTURE(sphereCenter);

        const double sphereRadius = 7.78421349144159;
        CAPTURE(sphereRadius);

        THEN("Return one intersection") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {{-4.91060527034701, -4.68576560570548, 5.67778090559027}});
        }
      }

      AND_WHEN("The sphere is a dot (the radius is 0) on the circle (which is not a dot)") {
        const arma::Col<double>::fixed<3>& circleCenter = {-3.50130736516486, 1.00721993762508, 4.45910241904761};
        CAPTURE(circleCenter);

        const double circleRadius = 5.99010703040300;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.176604220410152, -0.247642470535383, -0.952619628246476};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-4.91060527034701, -4.68576560570548, 5.67778090559027};
        CAPTURE(sphereCenter);

        const double sphereRadius = 0.0;
        CAPTURE(sphereRadius);

        THEN("Return one intersection") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {{-4.91060527034701, -4.68576560570548, 5.67778090559027}});
        }
      }

      AND_WHEN("Both the sphere and the circle are dots (both radii are 0), with an identical center") {
        const arma::Col<double>::fixed<3>& circleCenter = {-4.91060527034701, -4.68576560570548, 5.67778090559027};
        CAPTURE(circleCenter);

        const double circleRadius = 0.0;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.176604220410152, -0.247642470535383, -0.952619628246476};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-4.91060527034701, -4.68576560570548, 5.67778090559027};
        CAPTURE(sphereCenter);

        const double sphereRadius = 0.0;
        CAPTURE(sphereRadius);

        THEN("Return one intersection") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {{-4.91060527034701, -4.68576560570548, 5.67778090559027}});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is greater than *circleRadius + intersectionCircleRadius*") {
        const arma::Col<double>::fixed<3>& circleCenter = {2.63021058406213, -6.59774155884104, 7.57279366864463};
        CAPTURE(circleCenter);

        const double circleRadius = 3.86819367750765;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.176604220410152, -0.247642470535383, -0.952619628246476};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-4.26292172088363, 1.80075110090070, 1.42342000000000};
        CAPTURE(sphereCenter);

        const double sphereRadius = 7.78421349144159;
        CAPTURE(sphereRadius);

        THEN("Return zero intersections") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {});
        }
      }

      AND_WHEN("The sphere's intersection circle's center distance to the circle's center is less than *abs(circleRadius - intersectionCircleRadius)*") {
        const arma::Col<double>::fixed<3>& circleCenter = {-2.17603910288870, -1.50364082820138, 5.35751318672055};
        CAPTURE(circleCenter);

        const double circleRadius = 1.43225689874261;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.176604220410152, -0.247642470535383, -0.952619628246476};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {-4.26292172088363, 1.80075110090070, 1.42342000000000};
        CAPTURE(sphereCenter);

        const double sphereRadius = 7.78421349144159;
        CAPTURE(sphereRadius);

        THEN("Return zero intersections") {
          IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {});
        }
      }

      AND_WHEN("The sphere's intersection circle is identical (same radius and center) to the circle") {
        const arma::Col<double>::fixed<3>& circleCenter = {4.18240810714397, -4.140718315562911, 3.165517912607086};
        CAPTURE(circleCenter);

        const double circleRadius = 3.616619200562912;
        CAPTURE(circleRadius);

        const arma::Col<double>::fixed<3>& circleNormal = {0.057471463902417, -0.044336228909495, 0.997362185789595};
        CAPTURE(circleNormal);

        const arma::Col<double>::fixed<3>& sphereCenter = {4.0, -4.0, 0.0};
        CAPTURE(sphereCenter);

        const double sphereRadius = 4.81180968646967;
        CAPTURE(sphereRadius);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), std::logic_error);
        }
      }
    }

    WHEN("The sphere does not intersect with the circle's plane") {
      const arma::Col<double>::fixed<3>& circleCenter = {-2.17603910288870, -1.50364082820138, 15.35751318672055};
      CAPTURE(circleCenter);

      const double circleRadius = 1.43225689874261;
      CAPTURE(circleRadius);

      const arma::Col<double>::fixed<3>& circleNormal = {0.176604220410152, -0.247642470535383, -0.952619628246476};
      CAPTURE(circleNormal);

      const arma::Col<double>::fixed<3>& sphereCenter = {-4.26292172088363, 1.80075110090070, 1.42342000000000};
      CAPTURE(sphereCenter);

      const double sphereRadius = 7.78421349144159;
      CAPTURE(sphereRadius);

      THEN("Return zero intersections") {
        IS_EQUAL(mant::circleSphereIntersections(circleCenter, circleRadius, circleNormal, sphereCenter, sphereRadius), {});
      }
    }
  }
}
