// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

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
      CHECK(mant::itd::timeOfFlight(70, {137579269223.10669, 57556354100.251083, 272149.17967159057}, {-71760063272.15799, 79838850401.634674, 5233319855.1410503}, false) == Approx(62798681.8217061609));
      CHECK(mant::itd::timeOfFlight(10, {137579269223.10669, 57556354100.251083, 272149.17967159057}, {-71760063272.15799, 79838850401.634674, 5233319855.1410503}, true) == Approx(17385719.3169208393));
    }
  }
}

SCENARIO("positionAndVelocityOnOrbit", "[orbitalMechanics][positionAndVelocityOnOrbit]") {
  GIVEN("A modified Julian date, Keplerian elements and a modified Julian reference date") {
    WHEN("Valid timestamp and keplerian elements are given") {
      THEN("The eccentricity is less than 1.0") {
        auto positionAndVelocity = mant::itd::positionAndVelocityOnOrbit(4453401600.0, arma::vec({108209474522.8824, 0.0067767205622176587, 0.059248274299581655, 1.3383157887557473, 0.95858056664193103, 0.87043843382287189, 4453401600.0}));

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

SCENARIO("lambert", "[orbitalMechanics][lambert]") {
  GIVEN("A departure position, an arrival position, whether to use a prograde trajectory or not, a number of revolutions and a transfer time") {
    THEN("Return departure and arrival velocity") {
      auto departureAndArivalVelocity = mant::itd::lambert({137579269223.10669, 57556354100.251083, 272149.17967159057}, {-71760063272.15799, 79838850401.634674, 5233319855.1410503}, false, 0, 86486400.0);

      CHECK(arma::approx_equal(departureAndArivalVelocity.first, arma::vec({29130.37612361506, -22196.18767151564, -1637.863774140631}), "reldiff", ::mant::machinePrecision) == true);
      CHECK(arma::approx_equal(departureAndArivalVelocity.second, arma::vec({39181.9742085342, 22326.20307591167, 282.7788107589317}), "reldiff", ::mant::machinePrecision) == true);
    }
  }
}
