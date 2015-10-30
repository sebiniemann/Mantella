// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

class TestExponentialCorrelationFunction : public mant::ExponentialCorrelationFunction {
  public:
    using mant::ExponentialCorrelationFunction::ExponentialCorrelationFunction;
  
    double getCorrelationCoefficientImplementation(
        const arma::Col<double>& parameter) const override {
      return ExponentialCorrelationFunction::getCorrelationCoefficientImplementation(parameter);
    }
};

TEST_CASE("ExponentialCorrelationFunction") {
  const arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  TestExponentialCorrelationFunction exponentialCorrelationFunction(numberOfDimensions);
  
  SECTION(".getCorrelationCoefficientImplementation") {
    SECTION("Returns the exponential correlation coefficient.") {
      arma::Col<double> parameter = arma::abs(arma::normalise(getContinuousRandomNumbers(numberOfDimensions)));
      CAPTURE(parameter);
      
      CHECK(exponentialCorrelationFunction.getCorrelationCoefficientImplementation(parameter) == Approx(arma::prod(arma::exp(-parameter))));
    }
  }
  
  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(exponentialCorrelationFunction.toString() == "exponential_correlation_function");
    }
  }
}
