// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

class TestDistanceFunction : public mant::PNorm {
  public:
    using mant::PNorm::PNorm;
    
    double getLengthImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::PNorm::getLengthImplementation(parameter);
    }
};

TEST_CASE("DistanceFunction") {
  const arma::uword numberOfDimensions = getRandomNumberOfValues();
  CAPTURE(numberOfDimensions);
      
  const arma::uword p = getRandomNumberOfValues(1);
  CAPTURE(p);

  TestDistanceFunction distanceFunction(numberOfDimensions, p);

  SECTION(".numberOfDimensions_") {
    SECTION("The number of dimensions is public accessible and set by the constructor.") {
      CHECK(distanceFunction.numberOfDimensions_ == numberOfDimensions);
    }
  }

  SECTION(".getDistance") {
    SECTION("Returns the distance between two vectors.") {
      arma::Col<double> firstParameter = getRandomValues(numberOfDimensions, 1);
      CAPTURE(firstParameter);
      arma::Col<double> secondParameter = getRandomValues(numberOfDimensions, 1);
      CAPTURE(secondParameter);
      
      CHECK(distanceFunction.getDistance(firstParameter, secondParameter) == Approx(distanceFunction.getLength(secondParameter - firstParameter)));
    }
    
    // TODO Add exception tests
  }

  SECTION(".getLength") {
    SECTION("Returns the length of a vector.") {
      arma::Col<double> parameter = getRandomValues(numberOfDimensions, 1);
      CAPTURE(parameter);
      
      CHECK(distanceFunction.getLength(parameter) == Approx(distanceFunction.getLengthImplementation(parameter)));
    }
  }
}
