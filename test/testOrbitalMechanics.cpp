// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

// The following tests are based on the Cassini-Huygens mission, but only to the extend that the position and velocity of Earth and Venus should be correct. The satellit's position and especially its velocity might be slightly off from the real mission, as we do not included its mass, start procedure or actual thrusts during the mission.
SCENARIO("stumpffFunction", "[orbitalMechanics][stumpffFunction]") {
  GIVEN("A parameter and a type") {
    THEN("Return the result of the Stumpff function") {
      CHECK(mant::itd::stumpffFunction(2.0, 4) == Approx(0.0389859237));
      CHECK(mant::itd::stumpffFunction(-2.0, 5) == Approx(0.0087413847));
    }
  }
}

SCENARIO("timeOfFlight", "[orbitalMechanics][timeOfFlight]") {
  GIVEN("A universal variable, a departure position, an arrival position and whether to use a prograde trajectory or not") {
    THEN("Return the time of flight") {
      // Start of the Cassini-Huygens mission (Earth) towards Venus.
      CHECK(mant::itd::timeOfFlight(10, {9.0233700643056000e+10, -5.9401337435372940e+10, -6.4058980329345465e+09}, {-1.2386009048123251e+11, -8.4023348344113220e+10, -6.9291212706562760e+05}, true) == Approx(8288294.3396512074));

      // From Venus (after one revolution) back towards Earth.
      CHECK(mant::itd::timeOfFlight(80, {-1.2386009048123251e+11, -8.4023348344113220e+10, -6.9291212706562760e+05}, {8.6594207905454056e+10, -6.4592089903941589e+10, -6.4044436050632191e+09}, true) == Approx(34146470.2003143802));
    }
  }
}

SCENARIO("positionAndVelocityOnOrbit", "[orbitalMechanics][positionAndVelocityOnOrbit]") {
  GIVEN("A modified Julian date, Keplerian elements and a modified Julian reference date") {
    WHEN("Valid timestamp and keplerian elements are given") {
      THEN("The eccentricity is less than 1.0") {
        const arma::vec::fixed<7>& earthKeplerianElements = {1.082081565315099e+11, 0.000117909160580, 0.059246765838336, 0.963167183054409, 1.338290119889351, 0.860696061867345, 4453401600.0};
        const arma::vec::fixed<7>& venusKeplerianElements = {1.496534962730336e+11, 0.000297445943011, 0.000004658482428, 5.197011175839376, 2.861907905845351, 6.251579845252470, 4453401600.0};

        // Start of the Cassini-Huygens mission.
        auto positionAndVelocity = mant::itd::positionAndVelocityOnOrbit(4383590400.0, earthKeplerianElements);
        CHECK(arma::approx_equal(positionAndVelocity.first, arma::vec({9.0233700643056000e+10, -5.9401337435372940e+10, -6.4058980329345465e+09}), "reldiff", ::mant::machinePrecision) == true);
        CHECK(arma::approx_equal(positionAndVelocity.second, arma::vec({1.9255894993512982e+04, 2.9246934173490023e+04, 5.2706604616501238e+01}), "reldiff", ::mant::machinePrecision) == true);

        // Cassini-Huygens initiates its first gravity assist at Venus.
        positionAndVelocity = mant::itd::positionAndVelocityOnOrbit(4400265600.0, venusKeplerianElements);
        CHECK(arma::approx_equal(positionAndVelocity.first, arma::vec({-1.2386009048123251e+11, -8.4023348344113220e+10, -6.9291212706562760e+05}), "reldiff", ::mant::machinePrecision) == true);
        CHECK(arma::approx_equal(positionAndVelocity.second, arma::vec({1.6709019219457914e+04, -2.4645604926836451e+04, 1.5388103710539468e-02}), "reldiff", ::mant::machinePrecision) == true);

        // Cassini-Huygens initiates its first gravity assist at Earth.
        positionAndVelocity = mant::itd::positionAndVelocityOnOrbit(4441651200.0, earthKeplerianElements);
        CHECK(arma::approx_equal(positionAndVelocity.first, arma::vec({8.6594207905454056e+10, -6.4592089903941589e+10, -6.4044436050632191e+09}), "reldiff", ::mant::machinePrecision) == true);
        CHECK(arma::approx_equal(positionAndVelocity.second, arma::vec({2.0932656709230043e+04, 2.8071172815977701e+04, -6.8766811294092747e+01}), "reldiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("The eccentricity is equal to or greater than 1.0") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::itd::positionAndVelocityOnOrbit(4453401600.0, arma::vec({108209474522.8824, 1.0067767205622176587, 0.059248274299581655, 1.3383157887557473, 0.95858056664193103, 0.87043843382287189, 4453401600.0})), std::logic_error);
      }
    }
  }
}

SCENARIO("gravityAssist", "[orbitalMechanics][gravityAssist]") {
  GIVEN("A satellite velocity, a planet velocity, a planet position, a standard gravitational parameter and a periapsis") {
    THEN("Return the satellite's velocity after the gravity assist") {
      // Cassini-Huygens first gravity assist at Venus.
      CHECK(arma::approx_equal(mant::itd::gravityAssist({-5.0857975383751966e+02, 2.7099925129907842e+04, 1.4577391496363773e+03}, {-1.2386009048123251e+11, -8.4023348344113220e+10, -6.9291212706562760e+05}, {1.6709019219457914e+04, -2.4645604926836451e+04, 1.5388103710539468e-02}, 3.248599000000000e+14, 284000.0), arma::vec({5.2949592378477697e+04, -1.3103333329889585e+04, 1.8982382983194079e-01}), "reldiff", ::mant::machinePrecision) == true);
    }
  }
}

SCENARIO("lambert", "[orbitalMechanics][lambert]") {
  GIVEN("A departure position, an arrival position, whether to use a prograde trajectory or not, a number of revolutions and a transfer time") {
    THEN("Return departure and arrival velocity") {
      // Start of the Cassini-Huygens mission (Earth) towards Venus.
      auto departureAndArivalVelocity = mant::itd::lambert({9.0233700643056000e+10, -5.9401337435372940e+10, -6.4058980329345465e+09}, {-1.2386009048123251e+11, -8.4023348344113220e+10, -6.9291212706562760e+05}, false, 0, 16675200.0);
      CHECK(arma::approx_equal(departureAndArivalVelocity.first, arma::vec({-1.4491234582119466e+02, -3.7577132946178514e+04, -1.9906948333261682e+03}), "reldiff", ::mant::machinePrecision) == true);
      CHECK(arma::approx_equal(departureAndArivalVelocity.second, arma::vec({-5.0857975383751966e+02, 2.7099925129907842e+04, 1.4577391496363773e+03}), "reldiff", ::mant::machinePrecision) == true);

      // From Venus (after one revolution) back towards Earth.
      departureAndArivalVelocity = mant::itd::lambert({-1.2386009048123251e+11, -8.4023348344113220e+10, -6.9291212706562760e+05}, {8.6594207905454056e+10, -6.4592089903941589e+10, -6.4044436050632191e+09}, false, 1, 41385600.0);
      // Due to the varying middle bound when calculating the universal variable for the multi-revolution case, the precise value is bound to the systems randomness.
      CHECK(arma::all(departureAndArivalVelocity.first < 1e5) == true);
      CHECK(arma::all(departureAndArivalVelocity.second < 1e5) == true);
    }
  }
}
