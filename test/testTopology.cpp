// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <cmath>
#include <random>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("mahalanobisDistance") {
  const arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
    
  SECTION("Returns the mahalanobis distance.") { 
    const arma::Col<double>& firstParameter = getContinuousRandomNumbers(numberOfDimensions);
    CAPTURE(firstParameter);
    const arma::Col<double>& secondParameter = getContinuousRandomNumbers(numberOfDimensions);
    CAPTURE(secondParameter);
    
    // Generate a random covariance matrix
    arma::Mat<double> covariance = getContinuousRandomNumbers(numberOfDimensions, numberOfDimensions);
    covariance *= covariance.t();
    CAPTURE(covariance);
    
    CHECK(mant::mahalanobisDistance(firstParameter, secondParameter, covariance) == Approx(std::sqrt(arma::dot((firstParameter - secondParameter).t() * covariance.i(), firstParameter - secondParameter))));
  }

  SECTION("Exception tests:") {
    const arma::uword differentNumberOfDimensions = getDifferentDiscreteRandomNumber(numberOfDimensions);
    CAPTURE(differentNumberOfDimensions);
      
    SECTION("Throws an exception, if the number of dimensions between the first and second parameter is unequal.") {
      const arma::Col<double>& firstParameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(firstParameter);
      const arma::Col<double>& secondParameter = getContinuousRandomNumbers(differentNumberOfDimensions);
      CAPTURE(secondParameter);
      const arma::Mat<double> covariance = arma::eye<arma::Mat<double>>(numberOfDimensions, differentNumberOfDimensions);
      CAPTURE(covariance);
    
      CHECK_THROWS_AS(mant::mahalanobisDistance(firstParameter, secondParameter, covariance), std::logic_error);
    }
    
    SECTION("Throws an exception, if the number of dimensions of the first parameter and number of rows of the covariance matrix is unequal.") {
      const arma::Col<double>& firstParameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(firstParameter);
      const arma::Col<double>& secondParameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(secondParameter);
      const arma::Mat<double> covariance = arma::eye<arma::Mat<double>>(numberOfDimensions, differentNumberOfDimensions);
      CAPTURE(covariance);
    
      CHECK_THROWS_AS(mant::mahalanobisDistance(firstParameter, secondParameter, covariance), std::logic_error);
    }
    
    SECTION("Throws an exception, if the matrix is not a covariance matrix.") {
      SECTION("Throws an exception, if the covariance matrix is not square.") {
        const arma::Col<double>& firstParameter = getContinuousRandomNumbers(numberOfDimensions);
        CAPTURE(firstParameter);
        const arma::Col<double>& secondParameter = getContinuousRandomNumbers(numberOfDimensions);
        CAPTURE(secondParameter);
        const arma::Mat<double> covariance = arma::eye<arma::Mat<double>>(numberOfDimensions, differentNumberOfDimensions);
        CAPTURE(covariance);
      
        CHECK_THROWS_AS(mant::mahalanobisDistance(firstParameter, secondParameter, covariance), std::logic_error);
      }
      
      SECTION("Throws an exception, if the covariance matrix is not positive semi-definite.") {
        const arma::Col<double>& firstParameter = getContinuousRandomNumbers(numberOfDimensions);
        CAPTURE(firstParameter);
        const arma::Col<double>& secondParameter = getContinuousRandomNumbers(numberOfDimensions);
        CAPTURE(secondParameter);
        // A negated identity matrix is not positive semi-definite.
        const arma::Mat<double> covariance = -arma::eye<arma::Mat<double>>(numberOfDimensions, numberOfDimensions);
        CAPTURE(covariance);
        
        CHECK_THROWS_AS(mant::mahalanobisDistance(firstParameter, secondParameter, covariance), std::logic_error);
      }
    }
  }
}
