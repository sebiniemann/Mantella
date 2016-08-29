// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("Kriging.predict", "[Kriging][Kriging.predict]") {
  GIVEN("A kriging model trained on known test data") {
    WHEN("It predicts a selected known parameter") {
      THEN("The expected result is returned") {
        mant::Kriging kriging({});
      }
    }
  }
}
