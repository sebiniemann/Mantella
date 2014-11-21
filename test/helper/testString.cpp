// Catch
#include <catch.hpp>

// HOP
#include <hop>

TEST_CASE("String ends with", "") {
  CHECK(hop::endsWith("input.serialised", ".serialised") == true);
  CHECK(hop::endsWith("input.serialised", "serialised") == true);
  CHECK(hop::endsWith("input.serialised", ".serialise") == false);
  CHECK(hop::endsWith("input.serialised", ".mat") == false);
}

