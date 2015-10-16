// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

class TestGaussianCorrelationFunction : public mant::GaussianCorrelationFunction {
  public:
    using mant::GaussianCorrelationFunction::GaussianCorrelationFunction;
  
    double getCorrelationCoefficientImplementation(
        const arma::Col<double>& parameter) const override {
      return GaussianCorrelationFunction::getCorrelationCoefficientImplementation(parameter);
    }
};

TEST_CASE("GaussianCorrelationFunction") {
  const arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  TestGaussianCorrelationFunction gaussianCorrelationFunction(numberOfDimensions);
  
  SECTION(".getCorrelationCoefficientImplementation") {
    SECTION("Returns the exponential correlation coefficient.") {
      arma::Col<double> parameter = arma::abs(arma::normalise(getContinuousRandomNumbers(numberOfDimensions)));
      CAPTURE(parameter);
      
      CHECK(gaussianCorrelationFunction.getCorrelationCoefficientImplementation(parameter) == Approx(arma::prod(static_cast<arma::Col<double>>(arma::exp(-arma::pow(parameter, 2))))));
    }
  }
  
  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(gaussianCorrelationFunction.toString() == "gaussian_correlation_function");
    }
  }
}
