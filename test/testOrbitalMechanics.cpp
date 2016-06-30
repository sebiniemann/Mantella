// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("positionOnOrbit", "[orbitalMechanics][positionOnOrbit]") {
  GIVEN("Timestamp in mjd format and a 7-element vector with keplerian elements and a reference date in mjd format") {
    WHEN("Valid timestamp and keplerian elements are given") {
      THEN("Return position and velocity vector") {
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> positionAndVelocity = mant::itd::positionOnOrbit(4453401600.0, arma::vec({108209474522.8824, 0.0067767205622176587, 0.059248274299581655, 1.3383157887557473, 0.95858056664193103, 0.87043843382287189, 4453401600.0}));

        CAPTURE(positionAndVelocity.first - arma::vec({-1.0749198944842783e+11, -3.9372234530840988e+09, 6.1508375249456024e+09});
        CAPTURE(positionAndVelocity.second - arma::vec({1.0730577567788532e+03, -3.5152349789664826e+04, -5.4234241832146006e+02});
        
        CHECK(arma::approx_equal(positionAndVelocity.first, arma::vec({-1.0749198944842783e+11, -3.9372234530840988e+09, 6.1508375249456024e+09}), "absdiff", ::mant::machinePrecision) == true);
        CHECK(arma::approx_equal(positionAndVelocity.second, arma::vec({1.0730577567788532e+03, -3.5152349789664826e+04, -5.4234241832146006e+02}), "absdiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("Eccentricity is 1.0 or greater") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::itd::positionOnOrbit(54000.0, arma::vec({7.233268496749391e-01, 1.6755697267164094, 3.394589632336535e+00, 5.518541455452200e+01, 7.667837563371675e+01, 4.931425178852966e+01, 51544.0})), std::logic_error);
      }
    }
  }
}
