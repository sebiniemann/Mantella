// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// Mantella
#include <mantella>

TEST_CASE("quasiRandomSequence: getHaltonSequence(...)") {
  SECTION("Generates a Halton sequence without an offset.") {
    arma::Mat<double>::fixed<2, 5> expected = {
        0.0, 0.0, 1.0 / 2.0, 1.0 / 3.0, 1.0 / 4.0, 2.0 / 3.0, 3.0 / 4.0, 1.0 / 9.0, 1.0 / 8.0, 4.0 / 9.0,
    };
    IS_EQUAL(mant::getHaltonSequence({2, 3}, {0, 0}, 5), expected);
  }

  SECTION("Generates a Halton sequence with an offset.") {
    arma::Mat<double>::fixed<2, 5> expected = {
        3.0 / 4.0, 4.0 / 9.0, 1.0 / 8.0, 7.0 / 9.0, 5.0 / 8.0, 2.0 / 9.0, 3.0 / 8.0, 5.0 / 9.0, 7.0 / 8.0, 8.0 / 9.0,
    };
    IS_EQUAL(mant::getHaltonSequence({2, 3}, {3, 4}, 5), expected);
  }

  SECTION("Exception tests") {
    SECTION("Throw an exception, if the size of the base and seed mismatch.") {
      CHECK_THROWS_AS(mant::getHaltonSequence({1}, {3, 3}, 5), std::logic_error);
      CHECK_THROWS_AS(mant::getHaltonSequence({4, 5}, {3}, 6), std::logic_error);
    }
  }
}

TEST_CASE("quasiRandomSequence: getVanDerCorputSequence(...)") {
  SECTION("Generates a Van der Corput sequence without an offset.") {
    IS_EQUAL(mant::getVanDerCorputSequence(2, 0, 5), {0.0, 1.0 / 2.0, 1.0 / 4.0, 3.0 / 4.0, 1.0 / 8.0});
  }

  SECTION("Generates a Van der Corput sequence with an offset.") {
    IS_EQUAL(mant::getVanDerCorputSequence(3, 3, 5), {1.0 / 9.0, 4.0 / 9.0, 7.0 / 9.0, 2.0 / 9.0, 5.0 / 9.0});
  }
}
