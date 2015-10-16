// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

class TestPolynomialCorrelationFunction : public mant::PolynomialCorrelationFunction {
  public:
    using mant::PolynomialCorrelationFunction::PolynomialCorrelationFunction;
  
    double getCorrelationCoefficientImplementation(
        const arma::Col<double>& parameter) const override {
      return PolynomialCorrelationFunction::getCorrelationCoefficientImplementation(parameter);
    }
};

TEST_CASE("PolynomialCorrelationFunction") {
  const arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);

  const arma::uword polynomialOrder = std::uniform_int_distribution<arma::uword>(1, 3)(mant::Rng::getGenerator());
  CAPTURE(polynomialOrder);

  TestPolynomialCorrelationFunction polynomialCorrelationFunction(numberOfDimensions, polynomialOrder);

  SECTION(".polynomialOrder_") {
    SECTION("The polynomial order is public accessible and set by the constructor.") {
      CHECK(polynomialCorrelationFunction.polynomialOrder_ == polynomialOrder);
    }
  }
  
  SECTION(".getCorrelationCoefficientImplementation") {
    SECTION("Returns the first order polynomial correlation coefficient.") {
      TestPolynomialCorrelationFunction firstOrderPolynomialCorrelationFunction(numberOfDimensions, 1);
    
      arma::Col<double> parameter = arma::max(arma::zeros(numberOfDimensions), 1 - arma::abs(arma::normalise(getContinuousRandomNumbers(numberOfDimensions))));
      CAPTURE(parameter);
      
      CHECK(firstOrderPolynomialCorrelationFunction.getCorrelationCoefficientImplementation(parameter) == Approx(arma::prod(parameter)));
    }
    
    SECTION("Returns the second order polynomial correlation coefficient.") {
      TestPolynomialCorrelationFunction secondOrderPolynomialCorrelationFunction(numberOfDimensions, 2);
    
      arma::Col<double> parameter = arma::min(arma::ones(numberOfDimensions), arma::abs(arma::normalise(getContinuousRandomNumbers(numberOfDimensions))));
      CAPTURE(parameter);
      
      CHECK(secondOrderPolynomialCorrelationFunction.getCorrelationCoefficientImplementation(parameter) == Approx(arma::prod(1 - 1.5 * parameter + 0.5 * arma::pow(parameter, 3))));
    }
    
    SECTION("Returns the third order polynomial correlation coefficient.") {
      TestPolynomialCorrelationFunction thirdOrderPolynomialCorrelationFunction(numberOfDimensions, 3);
    
      arma::Col<double> parameter = arma::min(arma::ones(numberOfDimensions), arma::abs(arma::normalise(getContinuousRandomNumbers(numberOfDimensions))));
      CAPTURE(parameter);
      
      CHECK(thirdOrderPolynomialCorrelationFunction.getCorrelationCoefficientImplementation(parameter) == Approx(arma::prod(static_cast<arma::Col<double>>(1.0 - 3.0 * arma::pow(parameter, 2) + 2.0 * arma::pow(parameter, 3)))));
    }
  }
  
  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(polynomialCorrelationFunction.toString() == "polynomial_correlation_function");
    }
  }
}
