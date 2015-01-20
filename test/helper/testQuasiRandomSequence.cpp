// Catch
#include <catch.hpp>
#include <helper.hpp>

// Mantella
#include <mantella>

TEST_CASE("quasiRandomSequence: getHaltonSequence", "") {
  arma::Mat<double>::fixed<2, 5> expected;

  expected = {
    0.0, 0.0,
    1.0/2.0, 1.0/3.0,
    1.0/4.0, 2.0/3.0,
    3.0/4.0, 1.0/9.0,
    1.0/8.0, 4.0/9.0,
  };
  compare(mant::getHaltonSequence({2, 3}, {0, 0}, 5), expected);

  expected = {
    3.0/4.0, 1.0/9.0,
    1.0/8.0, 4.0/9.0,
    5.0/8.0, 7.0/9.0,
    3.0/8.0, 2.0/9.0,
    7.0/8.0, 5.0/9.0,
  };
  compare(mant::getHaltonSequence({2, 3}, {3, 3}, 5), expected);
}

TEST_CASE("quasiRandomSequence: getVanDerCorputSequence", "") {
  compare(mant::getVanDerCorputSequence(2, 0, 5), {0.0, 1.0/2.0, 1.0/4.0, 3.0/4.0, 1.0/8.0});
  compare(mant::getVanDerCorputSequence(3, 3, 5), {1.0/9.0, 4.0/9.0, 7.0/9.0, 2.0/9.0, 5.0/9.0});
}
