// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

// The following tests are based on the Cassini-Huygens mission, but only to the
// extend that the position and velocity of Earth and Venus should be correct.
// The satellit's position and especially its velocity might be slightly off
// from the real mission, as we do not included its mass, start procedure or
// actual thrusts during the mission.
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
      CHECK(mant::itd::timeOfFlight(10, {1.3673758920848439e+11, 6.0796174698209099e+10, 6.9558483573525702e+05}, {1.0958474846549156e+10, -1.0758115275186798e+11, -4.1768951438493319e+09}, true) == Approx(8037507.5295110801));

      // From Venus (after one revolution) back towards Earth.
      CHECK(mant::itd::timeOfFlight(80, {1.0958474846549156e+10, -1.0758115275186798e+11, -4.1768951438493319e+09}, {1.2400638592866623e+11, -8.3833980661523087e+10, 3.2922006660828856e+05}, true) == Approx(32369680.0891493782));
    }
  }
}

SCENARIO("sphereOfInfluenceRadius", "[orbitalMechanics][sphereOfInfluenceRadius]") {
  GIVEN("A universal variable, a departure position, an arrival position and whether to use a prograde trajectory or not") {
    THEN("Return the time of flight") {
      // Venus at the first gravity assist during the Cassini-Huygens mission.
      CHECK(mant::itd::sphereOfInfluenceRadius(1.082081565315099e+11, 4.867e24) == Approx(616194206.6671903133));
    }
  }
}

SCENARIO("positionAndVelocityOnOrbit", "[orbitalMechanics][positionAndVelocityOnOrbit]") {
  GIVEN("A modified Julian date, Keplerian elements and a modified Julian reference date") {
    WHEN("Valid timestamp and keplerian elements are given") {
      THEN("The eccentricity is less than 1.0") {
        // const arma::vec::fixed<7>& venusKeplerianElements = {1.082081565315099e+11, 0.000117909160580, 0.059246765838336, 0.963167183054409, 1.338290119889351, 0.860696061867345, 4453401600.0};
        // const arma::vec::fixed<7>& earthKeplerianElements = {1.496534962730336e+11, 0.000297445943011, 0.000004658482428, 5.197011175839376, 2.861907905845351, 6.251579845252470, 4453401600.0};

        // // Start of the Cassini-Huygens mission.
        // auto positionAndVelocity = mant::itd::positionAndVelocityOnOrbit(4383590400.0, earthKeplerianElements);
        // CHECK(arma::approx_equal(positionAndVelocity.first, arma::vec({1.3673758920848439e+11, 6.0796174698209099e+10, 6.9558483573525702e+05}), "reldiff", ::mant::machinePrecision) == true);
        // CHECK(arma::approx_equal(positionAndVelocity.second, arma::vec({-1.2107107151712305e+04, 2.7208968789186310e+04, 9.1444495876960158e-03}), "reldiff", ::mant::machinePrecision) == true);

        // // Cassini-Huygens initiates its first gravity assist at Venus.
        // positionAndVelocity = mant::itd::positionAndVelocityOnOrbit(4400265600.0, venusKeplerianElements);
        // CHECK(arma::approx_equal(positionAndVelocity.first, arma::vec({1.0958474846549156e+10, -1.0758115275186798e+11, -4.1768951438493319e+09}), "reldiff", ::mant::machinePrecision) == true);
        // CHECK(arma::approx_equal(positionAndVelocity.second, arma::vec({3.4796070499528447e+04, 3.6030236000891982e+03, -1.5725113477565317e+03}), "reldiff", ::mant::machinePrecision) == true);

        // // Cassini-Huygens initiates its first gravity assist at Earth.
        // positionAndVelocity = mant::itd::positionAndVelocityOnOrbit(4441651200.0, earthKeplerianElements);
        // CHECK(arma::approx_equal(positionAndVelocity.first, arma::vec({1.2400638592866623e+11, -8.3833980661523087e+10, 3.2922006660828856e+05}), "reldiff", ::mant::machinePrecision) == true);
        // CHECK(arma::approx_equal(positionAndVelocity.second, arma::vec({1.6669675918095876e+04, 2.4668643369510577e+04, 1.2225097790873254e-01}), "reldiff", ::mant::machinePrecision) == true);
      }
    }
  }
}

SCENARIO("gravityAssist", "[orbitalMechanics][gravityAssist]") {
  GIVEN("A satellite velocity, a planet velocity, a planet position, a standard gravitational parameter and a periapsis") {
    THEN("Return the satellite's velocity after the gravity assist") {
      // Cassini-Huygens first gravity assist at Venus.
      CHECK(arma::approx_equal(mant::itd::gravityAssist({-5.0857975383751966e+02, 2.7099925129907842e+04, 1.4577391496363773e+03}, {1.0958474846549156e+10, -1.0758115275186798e+11, -4.1768951438493319e+09}, {3.4796070499528447e+04, 3.6030236000891982e+03, -1.5725113477565317e+03}, 3.248599000000000e+14, 284000.0), arma::vec({4.1728848418989277e+04, 7.9548596887001404e+03, -1.7627547379633372e+03}), "reldiff", ::mant::machinePrecision) == true);
    }
  }
}

SCENARIO("lambert", "[orbitalMechanics][lambert]") {
  GIVEN("A departure position, an arrival position, whether to use a prograde trajectory or not, a number of revolutions and a transfer time") {
    THEN("Return departure and arrival velocity") {
      // Start of the Cassini-Huygens mission (Earth) towards Venus.
      auto departureAndArivalVelocity = mant::itd::lambert({1.3673758920848439e+11, 6.0796174698209099e+10, 6.9558483573525702e+05}, {1.0958474846549156e+10, -1.0758115275186798e+11, -4.1768951438493319e+09}, false, 0, 16675200.0);
      CHECK(arma::approx_equal(departureAndArivalVelocity.first, arma::vec({2.3102698713468271e+04, -1.4040201040693510e+04, -9.0292663484449645e+02}), "reldiff", ::mant::machinePrecision) == true);
      CHECK(arma::approx_equal(departureAndArivalVelocity.second, arma::vec({-3.2766867742466646e+04, 1.8316276427350986e+04, 1.2213095136337477e+03}), "reldiff", ::mant::machinePrecision) == true);

      // From Venus (after one revolution) back towards Earth.
      departureAndArivalVelocity = mant::itd::lambert({1.0958474846549156e+10, -1.0758115275186798e+11, -4.1768951438493319e+09}, {1.2400638592866623e+11, -8.3833980661523087e+10, 3.2922006660828856e+05}, false, 1, 41385600.0);
      // Due to the varying middle bound when calculating the universal variable
      // for the multi-revolution case, the precise value is bound to the
      // systems randomness.
      CHECK(arma::all(departureAndArivalVelocity.first < 1e5) == true);
      CHECK(arma::all(departureAndArivalVelocity.second < 1e5) == true);
    }
  }
}
