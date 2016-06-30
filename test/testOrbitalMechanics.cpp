// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("stumpffFunction", "[orbitalMechanics][stumpffFunction]") {
  GIVEN("A parameter and a type") {
    THEN("Return the result of the Stumpff function") {
      CHECK(mant::itd::stumpffFunction(2.0, 4) == Approx(-0.065180743));
      CHECK(mant::itd::stumpffFunction(-2.0, 5) == Approx(0.2962413847));
    }
  }
}

SCENARIO("timeOfFlight", "[orbitalMechanics][timeOfFlight]") {
  GIVEN("A universal variable, a departure position, an arrival position and wheter to use a prograde trajectory or not") {
    THEN("Return the time of flight") {
      CHECK(mant::itd::timeOfFlight(0.5, {-1.0749198944842783e+11, -3.9372234530840988e+09, 6.1508375249456024e+09}, {-0.9049198944842783e+11, -3.9382234530840988e+09, 5.5508375249456024e+09}, true) == Approx(0.0));
    }
  }
}

SCENARIO("positionAndVelocityOnOrbit", "[orbitalMechanics][positionAndVelocityOnOrbit]") {
  GIVEN("A modified Julian date, Keplerian elements and a modified Julian reference date") {
    WHEN("Valid timestamp and keplerian elements are given") {
      THEN("The eccentricity is less than 1.0") {
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> positionAndVelocity = mant::itd::positionAndVelocityOnOrbit(4453401600.0, arma::vec({108209474522.8824, 0.0067767205622176587, 0.059248274299581655, 1.3383157887557473, 0.95858056664193103, 0.87043843382287189, 4453401600.0}));

        CHECK(arma::approx_equal(positionAndVelocity.first, arma::vec({-1.0749198944842783e+11, -3.9372234530840988e+09, 6.1508375249456024e+09}), "reldiff", ::mant::machinePrecision) == true);
        CHECK(arma::approx_equal(positionAndVelocity.second, arma::vec({1.0730577567788532e+03, -3.5152349789664826e+04, -5.4234241832146006e+02}), "reldiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("The eccentricity is equal to or greater than 1.0") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::itd::positionAndVelocityOnOrbit(4453401600.0, arma::vec({108209474522.8824, 1.0067767205622176587, 0.059248274299581655, 1.3383157887557473, 0.95858056664193103, 0.87043843382287189, 4453401600.0})), std::logic_error);
      }
    }
  }
}

SCENARIO("positionAndVelocityOnOrbit", "[orbitalMechanics][positionAndVelocityOnOrbit]") {
  GIVEN("A modified Julian date, Keplerian elements and a modified Julian reference date") {
    WHEN("Valid timestamp and keplerian elements are given") {
      THEN("The eccentricity is less than 1.0") {
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> positionAndVelocity = mant::itd::positionAndVelocityOnOrbit(4453401600.0, arma::vec({108209474522.8824, 0.0067767205622176587, 0.059248274299581655, 1.3383157887557473, 0.95858056664193103, 0.87043843382287189, 4453401600.0}));

        CHECK(arma::approx_equal(positionAndVelocity.first, arma::vec({-1.0749198944842783e+11, -3.9372234530840988e+09, 6.1508375249456024e+09}), "reldiff", ::mant::machinePrecision) == true);
        CHECK(arma::approx_equal(positionAndVelocity.second, arma::vec({1.0730577567788532e+03, -3.5152349789664826e+04, -5.4234241832146006e+02}), "reldiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("The eccentricity is equal to or greater than 1.0") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::itd::positionAndVelocityOnOrbit(4453401600.0, arma::vec({108209474522.8824, 1.0067767205622176587, 0.059248274299581655, 1.3383157887557473, 0.95858056664193103, 0.87043843382287189, 4453401600.0})), std::logic_error);
      }
    }
  }
}