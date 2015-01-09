// Catch
#include <catch.hpp>

// Mantella
#include <mantella>

TEST_CASE("String ends with", "") {
  CHECK(mant::endsWith("input.serialised", ".serialised") == true);
  CHECK(mant::endsWith("input.serialised", "serialised") == true);
  CHECK(mant::endsWith("input.serialised", ".serialise") == false);
  CHECK(mant::endsWith("input.serialised", ".mat") == false);
}

