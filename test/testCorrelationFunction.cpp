// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

class TestCorrelationFunction : public mant::ExponentialCorrelationFunction {
  public:
    using mant::ExponentialCorrelationFunction::ExponentialCorrelationFunction;

  double getCorrelationCoefficientImplementation(
      const arma::Col<double>& parameter) const override {
    return ExponentialCorrelationFunction::getCorrelationCoefficientImplementation(parameter);
  }
};

TEST_CASE("CorrelationFunction") {
  const arma::uword numberOfDimensions = getRandomNumberOfValues();
  CAPTURE(numberOfDimensions);
  
  TestCorrelationFunction correlationFunction(numberOfDimensions);
  
  SECTION(".getCorrelationCoefficient") {
    SECTION("Returns the exponential correlation coefficient.") {
      arma::Col<double> parameter = arma::max(arma::zeros(numberOfDimensions), 1 - arma::abs(arma::normalise(getRandomValues(numberOfDimensions, 1))));
      CAPTURE(parameter);
      
      CHECK(correlationFunction.getCorrelationCoefficient(parameter) == Approx(correlationFunction.getCorrelationCoefficient(parameter)));
    }
  }
  
  SECTION("setCorrelationWeights") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      // CHECK(correlationFunction.toString() == "spline_correlation_function");
    }
  }
  
  SECTION("getCorrelationWeights") {
    SECTION("Returns the correlation weighting.") {
      // CHECK(correlationFunction.toString() == "spline_correlation_function");
    }
  }
}
