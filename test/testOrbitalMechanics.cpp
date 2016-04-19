// Armadillo
#include <armadillo>

// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("orbitOnPosition", "[orbitalMechanics][orbitOnPosition]") {
  GIVEN("Timestamp in mjd format and a 7-element vector with keplerian elements and a reference date in mjd format") {
    arma::Col<double>::fixed<7> keplerianElementsAstroidData = {3.9501468, 0.2391642, 6.87574, 16.88982, 48.9603, 229.49648, 54000};

    WHEN("Valid timestamp and keplerian elements are given") {
      THEN("Return position and velocity vector") {
        double mjd = 51544.0;
        arma::Col<double>::fixed<3> expectedPosition = {-5.79925e+11, 1.05904e+11, 3.25362e+10};
        arma::Col<double>::fixed<3> expectedVelocity = {-6209.06, -13586.8, -1350.16};

        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> resultPair = mant::itd::orbitOnPosition(mjd, keplerianElementsAstroidData);

        IS_EQUAL(resultPair.first, expectedPosition);
        IS_EQUAL(resultPair.second, expectedVelocity);
      }
    }

    WHEN("The modifiedJulianDay is infinite") {
      THEN("Throw a std::logic_error") {
        double mjd = arma::datum::inf;

        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd, keplerianElementsAstroidData), std::logic_error);
      }
    }

    WHEN("Eccentricity is 1.0 or greater") {
      THEN("Throw a std::logic_error") {
        arma::Col<double>::fixed<7> keplerianElementsAstroidDataHighEccentricity = {3.9501468, 1.2391642, 6.87574, 16.88982, 48.9603, 229.49648, 54000.0};
        double mjd = 54000.0 + continuousRandomNumber();

        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd, keplerianElementsAstroidDataHighEccentricity), std::logic_error);
      }
    }
  }
}
