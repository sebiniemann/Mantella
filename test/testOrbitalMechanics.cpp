// Armadillo
#include <armadillo>

// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("orbitOnPosition", "[orbitalMechanics][orbitOnPosition]") {
  GIVEN("Timestamp in mjd2000 format and (2,6)-Matrix with keplerian elements") {
    arma::Mat<double>::fixed<2, 6> keplerianElementsVenus =
        {{0.72333566, 0.00677672, 3.39467605, 181.97909950, 131.60246718, 76.67984255},
         {0.00000390, -0.00004107, -0.00078890, 58517.81538729, 0.00268329, -0.27769418}};

    WHEN("Valid timestamp and keplerian elements are given") {
      THEN("Return position and velocity vector") {
        arma::Col<double>::fixed<3> expectedPosition = {1.08442e+11, -4.42187e+09, -6.31965e+09};
        arma::Col<double>::fixed<3> expectedVelocity = {1272.54, 34831.5, 402.894};

        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> resultPair = mant::itd::orbitOnPosition(1234.0, keplerianElementsVenus);

        IS_EQUAL(resultPair.first, expectedPosition);
        IS_EQUAL(resultPair.second, expectedVelocity);
      }
    }

    WHEN("Timestamp is not in range (-73048.0, 18263.0)") {
      THEN("Throw a std::logic_error") {
        double mjd2000 = -73048.0 - std::abs(continuousRandomNumber());
        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd2000, keplerianElementsVenus), std::logic_error);

        mjd2000 = 18263.0 + std::abs(continuousRandomNumber());
        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd2000, keplerianElementsVenus), std::logic_error);

        mjd2000 = arma::datum::inf;
        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd2000, keplerianElementsVenus), std::logic_error);
      }
    }

    WHEN("Eccentricity is 1.0 or greater") {
      THEN("Throw a std::logic_error") {
        arma::Mat<double>::fixed<2, 6> keplerianElementsVenusHighEccentricity =
            {{0.72333566, 1.20677672, 3.39467605, 181.97909950, 131.60246718, 76.67984255},
             {0.00000390, -0.00004107, -0.00078890, 58517.81538729, 0.00268329, -0.27769418}};

        double mjd2000 = std::abs(continuousRandomNumber());
        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd2000, keplerianElementsVenusHighEccentricity), std::logic_error);
      }
    }
  }

  GIVEN("Timestamp in mjd2000 format and a 7-element vector with keplerian elements and a reference date in mjd format") {
    arma::Col<double>::fixed<7> keplerianElementsAstroidData = {3.9501468, 0.2391642, 6.87574, 16.88982, 48.9603, 229.49648, 54000};

    WHEN("Valid timestamp and keplerian elements are given") {
      THEN("Return position and velocity vector") {
        arma::Col<double>::fixed<3> expectedPosition = {-3.56269e+11, -6.3588e+11, -6.08879e+10};
        arma::Col<double>::fixed<3> expectedVelocity = {10105.4, -5925.48, -1037.72};

        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> resultPair = mant::itd::orbitOnPosition(1234.0, keplerianElementsAstroidData);

        IS_EQUAL(resultPair.first, expectedPosition);
        IS_EQUAL(resultPair.second, expectedVelocity);
      }
    }

    WHEN("Timestamp is not in range (-73048.0, 18263.0)") {
      THEN("Throw a std::logic_error") {
        double mjd2000 = -73048.0 - std::abs(continuousRandomNumber());
        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd2000, keplerianElementsAstroidData), std::logic_error);

        mjd2000 = 18263.0 + std::abs(continuousRandomNumber());
        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd2000, keplerianElementsAstroidData), std::logic_error);

        mjd2000 = arma::datum::inf;
        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd2000, keplerianElementsAstroidData), std::logic_error);
      }
    }

    WHEN("Eccentricity is 1.0 or greater") {
      THEN("Throw a std::logic_error") {
        arma::Col<double>::fixed<7> keplerianElementsAstroidDataHighEccentricity = {3.9501468, 1.2391642, 6.87574, 16.88982, 48.9603, 229.49648, 54000};

        double mjd2000 = std::abs(continuousRandomNumber());
        CHECK_THROWS_AS(mant::itd::orbitOnPosition(mjd2000, keplerianElementsAstroidDataHighEccentricity), std::logic_error);
      }
    }
  }
}
