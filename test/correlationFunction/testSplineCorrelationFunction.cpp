// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

class TestSplineCorrelationFunction : public mant::SplineCorrelationFunction {
  public:
    using mant::SplineCorrelationFunction::SplineCorrelationFunction;
  
    double getCorrelationCoefficientImplementation(
        const arma::Col<double>& parameter) const override {
      return SplineCorrelationFunction::getCorrelationCoefficientImplementation(parameter);
    }
};

TEST_CASE("SplineCorrelationFunction") {
  const arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  TestSplineCorrelationFunction splineCorrelationFunction(numberOfDimensions);
  
  SECTION(".getCorrelationCoefficientImplementation") {
    SECTION("Returns the exponential correlation coefficient.") {
      arma::Col<double> parameter = arma::abs(arma::normalise(getContinuousRandomNumbers(numberOfDimensions)));
      CAPTURE(parameter);
      
      double expectedCorrelationCoefficient = 0;
      if (arma::all(parameter < 1)) {
        const arma::Col<arma::uword>& firstPart = arma::find(parameter <= 0.2);
        const arma::Col<arma::uword>& secondPart = arma::find(0.2 < parameter && parameter < 1);

        expectedCorrelationCoefficient = 1;
        expectedCorrelationCoefficient *= arma::prod(1 - 15 * arma::pow(parameter.elem(firstPart), 2.0) + 30 * arma::pow(parameter.elem(firstPart), 3.0));
        expectedCorrelationCoefficient *= arma::prod(1.25 * arma::pow(1.0 - parameter.elem(secondPart), 3.0));
      }
      
      CHECK(splineCorrelationFunction.getCorrelationCoefficientImplementation(parameter) == Approx(expectedCorrelationCoefficient));
    }
  }
  
  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(splineCorrelationFunction.toString() == "spline_correlation_function");
    }
  }
}
