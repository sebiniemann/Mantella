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
