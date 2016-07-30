// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("rotationMatrix2d", "[geometry][rotationMatrix2d]") {
  GIVEN("An angle") {
    WHEN("The angle is finite") {
      THEN("Return a 2-dimensional rotation matrix") {
        CHECK(arma::approx_equal(mant::rotationMatrix2d(1.0), arma::mat::fixed<2, 2>({{0.540302305868140, -0.841470984807897}, {0.841470984807897, 0.540302305868140}}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }
}

SCENARIO("rotationMatrix3d", "[geometry][rotationMatrix3d]") {
  GIVEN("A roll, pitch and yaw angle") {
    WHEN("The angles are finite") {
      THEN("Return a 3-dimensional rotation matrix") {
        CHECK(arma::approx_equal(mant::rotationMatrix3d(-1.0, 2.0, -3.0), arma::mat::fixed<3, 3>({{0.411982245665683, 0.833737651774157, -0.367630462924899}, {0.058726644927621, -0.426917621276207, -0.902381585483331}, {-0.909297426825682, 0.350175488374015, -0.224845095366153}}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }
}

SCENARIO("circleCircleIntersections", "[geometry][circleCircleIntersections]") {
  GIVEN("Two 2-dimensional circle centers and radii") {
    WHEN("The centers distance is within `(max(firstRadius, secondRadius), firstRadius + secondRadius)`") {
      THEN("Return 2 intersections") {
        CHECK(mant::circleCircleIntersections({-1.2, -2.4}, 3.2, {2.2, 1.4}, 4.1) == std::vector<arma::vec::fixed<2>>({{-1.8455795693519, 0.73420277257798}, {1.986425723198, -2.6944335418087}}));
      }
    }

    WHEN("The centers distance is within `(abs(firstRadius - secondRadius), max(firstRadius, secondRadius))`") {
      THEN("Return 2 intersections") {
        CHECK(mant::circleCircleIntersections({-1.2, -2.4}, 3.2, {-0.1, 0.2}, 1.1) == std::vector<arma::vec::fixed<2>>({{-1.0250514584014, 0.79521407855444}, {0.97135007822576, -0.049417340787819}}));
      }
    }

    WHEN("The centers distance is exactly `firstRadius + secondRadius`") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleCircleIntersections({2.5, 1.2}, 3.2, {-1.3, 1.2}, 0.6) == std::vector<arma::vec::fixed<2>>({{-0.7, 1.2}}));
      }
    }

    WHEN("The centers distance is exactly `abs(firstRadius - secondRadius)`") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleCircleIntersections({2.5, 1.2}, 3.2, {5.2, 1.2}, 0.5) == std::vector<arma::vec::fixed<2>>({{5.7, 1.2}}));
      }
    }

    WHEN("One circle is a dot (the radius is 0 and The distance is equal to `firstRadius`) on the other circle (which is not a dot)") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleCircleIntersections({2.5, 1.2}, 3.2, {5.7, 1.2}, 0.0) == std::vector<arma::vec::fixed<2>>({{5.7, 1.2}}));
        CHECK(mant::circleCircleIntersections({5.7, 1.2}, 0.0, {2.5, 1.2}, 3.2) == std::vector<arma::vec::fixed<2>>({{5.7, 1.2}}));
      }
    }

    WHEN("Both circles are dots (both radii are 0) with an identical center") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleCircleIntersections({0.1, 6.36}, 0.0, {0.1, 6.36}, 0.0) == std::vector<arma::vec::fixed<2>>({{0.1, 6.36}}));
      }
    }

    WHEN("The centers distance is greater than `firstRadius + secondRadius`") {
      THEN("Return 0 intersections") {
        CHECK(mant::circleCircleIntersections({2.5, 1.2}, 3.2, {20.5, 10.2}, 1.2).empty() == true);
      }
    }

    WHEN("The centers distance is less than `abs(firstRadius - secondRadius)`") {
      THEN("Return 0 intersections") {
        CHECK(mant::circleCircleIntersections({2.5, 1.2}, 3.2, {2.8, 1.6}, 0.6).empty() == true);
      }
    }
  }
}

SCENARIO("circleSphereIntersections", "[geometry][circleSphereIntersections]") {
  GIVEN("A 3-dimensional circle center, radius and normal, as well as a 3-dimensional sphere center and radius") {
    WHEN("The sphere's intersection circle's center distance to the circle's center is within `(max(circleRadius, intersectionCircleRadius), circleRadius + intersectionCircleRadius)`") {
      THEN("Return 2 intersections") {
        CHECK(mant::circleSphereIntersections({1.71037554404510, 3.64627141711538, 7.78840646877250}, 8.08132872957838, {0.285511339171737, 0.263199560882686, -0.921525510420370}, {-3.44040300957287, -4.17124887185550, 0.875266289505627}, 6.46975191235126) == std::vector<arma::vec::fixed<3>>({{1.16469480073837, -4.06037146073060, 5.41822459489890}, {-5.18238295082404, 0.666644704628342, 4.80184004206240}}));
      }
    }

    WHEN("The sphere's intersection circle's center distance to the circle's center is within `(abs(circleRadius - intersectionCircleRadius), max(circleRadius, intersectionCircleRadius))`") {
      THEN("Return 2 intersections") {
        CHECK(mant::circleSphereIntersections({-3.50130736516486, 1.00721993762508, 4.45910241904761}, 5.99010703040300, {0.176604220410152, -0.247642470535383, -0.952619628246476}, {-4.26292172088363, 1.80075110090070, 1.42342000000000}, 7.78421349144159) == std::vector<arma::vec::fixed<3>>({{-4.91060527034701, -4.68576560570548, 5.67778090559027}, {2.39261228050498, 1.42557676033646, 5.44300841809502}}));
      }
    }

    WHEN("The sphere's intersection circle's center distance to the circle's center is exactly `circleRadius + intersectionCircleRadius`") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleSphereIntersections({2.63021058406213, -6.59774155884104, 7.57279366864463}, 4.868193677507644, {0.176604220410152, -0.247642470535383, -0.952619628246476}, {-4.26292172088363, 1.80075110090070, 1.42342000000000}, 7.78421349144159) == std::vector<arma::vec::fixed<3>>({{-0.296249423553824, -2.99905684451752, 6.09475110077178}}));
      }
    }

    WHEN("The sphere's intersection circle's center distance to the circle's center is exactly `abs(circleRadius - intersectionCircleRadius)`") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleSphereIntersections({-2.17603910288870, -1.50364082820138, 5.35751318672055}, 2.58770224454292, {0.090410660062076, -0.126777939782578, -0.987802240598605}, {-4.26292172088363, 1.80075110090070, 1.42342000000000}, 7.78421349144159) == std::vector<arma::vec::fixed<3>>({{-0.797087405472182, -3.65601747058010, 5.75996804375059}}));
      }
    }

    WHEN("The sphere's intersection circle is a dot (the radius is 0) on the circle (which is not a dot)") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleSphereIntersections({-3.89883315146586, 4.61891105015065, 7.97098605210794}, 12.7117514791544, {0.586302739220448, -0.563800193419407, 0.581703051292357}, {-2.28880000000000, 1.88432000000000, -5.43603734569035}, 5.31317660930921) == std::vector<arma::vec::fixed<3>>({{0.826330000000000, -1.11125000000000, -2.34534630000000}}));
      }
    }

    WHEN("The circle is a dot (the radius is 0) on the sphere (which is not a dot)") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleSphereIntersections({-4.91060527034701, -4.68576560570548, 5.67778090559027}, 0.0, {0.176604220410152, -0.247642470535383, -0.952619628246476}, {-4.26292172088363, 1.80075110090070, 1.42342000000000}, 7.78421349144159) == std::vector<arma::vec::fixed<3>>({{-4.91060527034701, -4.68576560570548, 5.67778090559027}}));
      }
    }

    WHEN("The sphere is a dot (the radius is 0) on the circle (which is not a dot)") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleSphereIntersections({-3.50130736516486, 1.00721993762508, 4.45910241904761}, 5.99010703040300, {0.176604220410152, -0.247642470535383, -0.952619628246476}, {-4.91060527034701, -4.68576560570548, 5.67778090559027}, 0.0) == std::vector<arma::vec::fixed<3>>({{-4.91060527034701, -4.68576560570548, 5.67778090559027}}));
      }
    }

    WHEN("Both the sphere and the circle are dots (both radii are 0), with an identical center") {
      THEN("Return 1 intersection") {
        CHECK(mant::circleSphereIntersections({-4.91060527034701, -4.68576560570548, 5.67778090559027}, 0.0, {0.176604220410152, -0.247642470535383, -0.952619628246476}, {-4.91060527034701, -4.68576560570548, 5.67778090559027}, 0.0) == std::vector<arma::vec::fixed<3>>({{-4.91060527034701, -4.68576560570548, 5.67778090559027}}));
      }
    }

    WHEN("The sphere's intersection circle's center distance to the circle's center is greater than `circleRadius + intersectionCircleRadius`") {
      THEN("Return 0 intersections") {
        CHECK(mant::circleSphereIntersections({2.63021058406213, -6.59774155884104, 7.57279366864463}, 3.86819367750765, {0.176604220410152, -0.247642470535383, -0.952619628246476}, {-4.26292172088363, 1.80075110090070, 1.42342000000000}, 7.78421349144159).empty() == true);
      }
    }

    WHEN("The sphere's intersection circle's center distance to the circle's center is less than `abs(circleRadius - intersectionCircleRadius)`") {
      THEN("Return 0 intersections") {
        CHECK(mant::circleSphereIntersections({-2.17603910288870, -1.50364082820138, 5.35751318672055}, 1.43225689874261, {0.176604220410152, -0.247642470535383, -0.952619628246476}, {-4.26292172088363, 1.80075110090070, 1.42342000000000}, 7.78421349144159).empty() == true);
      }
    }

    WHEN("The sphere does not intersect with the circle's plane") {
      THEN("Return 0 intersections") {
        CHECK(mant::circleSphereIntersections({-2.17603910288870, -1.50364082820138, 15.35751318672055}, 1.43225689874261, {0.176604220410152, -0.247642470535383, -0.952619628246476}, {-4.26292172088363, 1.80075110090070, 1.42342000000000}, 7.78421349144159).empty() == true);
      }
    }
  }
}
