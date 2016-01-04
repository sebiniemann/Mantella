// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <array>

// Mantella
#include <mantella>

TEST_CASE("multiquadraticRadialBasisFunctionValue") {
  const double distance = getContinuousRandomNumber();
  CAPTURE(distance);
  const double exponent = getContinuousRandomNumber();
  CAPTURE(exponent);

  SECTION("Calculates the multiquadratic radial basis function value.") {
    // Radial basis functions must return the same value for the negated distance.
    const double expected = std::sqrt(1 + std::pow(exponent * distance, 2));

    CHECK(mant::multiquadraticRadialBasisFunctionValue(distance, exponent) == Approx(expected));
    CHECK(mant::multiquadraticRadialBasisFunctionValue(-distance, exponent) == Approx(expected));
  }
}

TEST_CASE("gaussianRadialBasisFunctionValue") {
  const double distance = getContinuousRandomNumber();
  CAPTURE(distance);
  const double exponent = getContinuousRandomNumber();
  CAPTURE(exponent);

  SECTION("Calculates the Gaussian radial basis function value.") {
    // Radial basis functions must return the same value for the negated distance.
    const double expected = std::exp(-std::pow(exponent * distance, 2));

    CHECK(mant::gaussianRadialBasisFunctionValue(distance, exponent) == Approx(expected));
    CHECK(mant::gaussianRadialBasisFunctionValue(-distance, exponent) == Approx(expected));
  }
}

TEST_CASE("polyharmonicSplineRadialBasisFunctionValue") {
  const double distance = getContinuousRandomNumber();
  CAPTURE(distance);
  const arma::uword polynomialOrder = 2 * getDiscreteRandomNumber();
  CAPTURE(polynomialOrder);

  SECTION("Calculates the polyharmonic spline radial basis function value for even polynomial orders.") {
    // Radial basis functions must return the same value for the negated distance.
    const double expected = std::pow(std::abs(distance), polynomialOrder) * std::log(std::abs(distance));

    CHECK(mant::polyharmonicSplineRadialBasisFunctionValue(distance, polynomialOrder) == Approx(expected));
    CHECK(mant::polyharmonicSplineRadialBasisFunctionValue(-distance, polynomialOrder) == Approx(expected));
  }

  SECTION("Calculates the polyharmonic spline radial basis function value for even polynomial orders.") {
    const arma::uword unevenPolynomialOrder = polynomialOrder + 1;
    CAPTURE(unevenPolynomialOrder);

    // Radial basis functions must return the same value for the negated distance.
    const double expected = std::pow(std::abs(distance), unevenPolynomialOrder);

    CHECK(mant::polyharmonicSplineRadialBasisFunctionValue(distance, unevenPolynomialOrder) == Approx(expected));
    CHECK(mant::polyharmonicSplineRadialBasisFunctionValue(-distance, unevenPolynomialOrder) == Approx(expected));
  }
}
