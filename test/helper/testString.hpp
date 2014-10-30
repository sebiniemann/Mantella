#pragma once

// Catch
#include <catch/catch.hpp>

// HOP
#include <hop>

TEST_CASE("String helper", "[string]") {

  SECTION("Test impementation correctness") {
    CHECK(hop::endsWith("input.serialised", ".serialised") == true);
    CHECK(hop::endsWith("input.serialised", "serialised") == true);
    CHECK(hop::endsWith("input.serialised", ".serialise") == false);
    CHECK(hop::endsWith("input.serialised", ".mat") == false);
  }
}

