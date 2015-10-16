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
  const arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  TestCorrelationFunction correlationFunction(numberOfDimensions);
  
  SECTION(".getCorrelationCoefficient") {
    SECTION("Returns the exponential correlation coefficient.") {
      arma::Col<double> parameter = arma::max(arma::zeros(numberOfDimensions), 1 - arma::abs(arma::normalise(getContinuousRandomNumbers(numberOfDimensions))));
      CAPTURE(parameter);
      
      CHECK(correlationFunction.getCorrelationCoefficient(parameter) == Approx(correlationFunction.getCorrelationCoefficient(parameter)));
    }
  }
  
  SECTION("setCorrelationWeights") {
    SECTION(".") {
      
    }
  }
  
  SECTION("getCorrelationWeights") {
    SECTION("Returns the correlation weighting.") {
      
    }
  }
}
