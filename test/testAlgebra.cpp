// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

TEST_CASE("polynomial") {
  const arma::Col<double>& parameter = {2, -3};
  CAPTURE(parameter);

  SECTION("Returns a constant polynomial.") {
    IS_EQUAL(mant::polynomial(parameter, 0), {1});
  }

  SECTION("Returns a linear polynomial.") {
    HAS_SAME_ELEMENTS(mant::polynomial(parameter, 1), {2, -3 /* Linear */, 1 /* Constant */});
  }

  SECTION("Returns a quadratic polynomial.") {
    HAS_SAME_ELEMENTS(mant::polynomial(parameter, 2), {4, -6, 9 /* Quadratic */, 2, -3 /* Linear */, 1 /* Constant */});
  }

  SECTION("Returns a cubic polynomial.") {
    HAS_SAME_ELEMENTS(mant::polynomial(parameter, 3), {8, -12, 18, -27 /* Cubic */, 4, -6, 9 /* Quadratic */, 2, -3 /* Linear */, 1 /* Constant */});
  }

  SECTION("Returns a 5-dimensional polynomial.") {
    HAS_SAME_ELEMENTS(mant::polynomial(parameter, 5), {32, -48, 72, -108, 162, -243 /* 5-dimensional */, 16, -24, 36, -54, 81 /* 4-dimensional */, 8, -12, 18, -27 /* Cubic */, 4, -6, 9 /* Quadratic */, 2, -3 /* Linear */, 1 /* Constant */});
  }
}
