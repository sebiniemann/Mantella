// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

class TestMahalanobisDistance : public mant::MahalanobisDistance {
  public:
    using mant::MahalanobisDistance::MahalanobisDistance;
    
    double getLengthImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::MahalanobisDistance::getLengthImplementation(parameter);
    }
};

TEST_CASE("MahalanobisDistance") {
  const arma::uword numberOfDimensions = getRandomNumberOfValues();
  CAPTURE(numberOfDimensions);
      
  arma::Mat<double> covariance = getRandomValues(numberOfDimensions, numberOfDimensions);
  covariance *= covariance.t();
  covariance= arma::symmatu(covariance);
  CAPTURE(covariance);

  TestMahalanobisDistance mahalanobisDistance(covariance);

  SECTION(".covariance_") {
    SECTION("The covariance matrix is public accessible and set by the constructor.") {
      IS_EQUAL(mahalanobisDistance.covariance_, covariance);
    }
  }

  SECTION(".getLengthImplementation") {
    SECTION("Returns the mahalanobis-distance-based length of a vector.") {
      arma::Col<double> parameter = getRandomValues(numberOfDimensions, 1);
      CAPTURE(parameter);
      
      CHECK(mahalanobisDistance.getLengthImplementation(parameter) == Approx(std::sqrt(arma::dot(parameter, arma::solve(covariance, parameter)))));
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(mahalanobisDistance.toString() == "mahalanobis_distance");
    }
  }
  
    // TODO Add exception tests (covariance must be square, symmetric and positive semi-definite)
}
