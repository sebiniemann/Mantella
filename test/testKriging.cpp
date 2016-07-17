// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("Kriging.predict", "[Kriging][Kriging.predict]") {
  GIVEN("A kriging model that has not been initialized with `.train()`") {
    WHEN("`.predict()` is called") {
      THEN("throw a domain_error") {
      }
    }
  }

  GIVEN("Any trained kriging model") {
    WHEN("It predicts a value from the samples set") {
      THEN("The exact sample value is returned") {
      }
    }
  }

  GIVEN("A kriging model trained on known test data") {
    WHEN("It predicts a selected known parameter") {
      THEN("The expected result is returned") {
      }
    }
  }
}
