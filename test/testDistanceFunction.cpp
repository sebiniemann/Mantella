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
  const arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
      
  const arma::uword p = 1 + getDiscreteRandomNumber();
  CAPTURE(p);

  TestDistanceFunction distanceFunction(numberOfDimensions, p);

  SECTION(".numberOfDimensions_") {
    SECTION("The number of dimensions is public accessible and set by the constructor.") {
      CHECK(distanceFunction.numberOfDimensions_ == numberOfDimensions);
    }
  }

  SECTION(".getDistance") {
    SECTION("Returns the distance between two vectors.") {
      arma::Col<double> firstParameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(firstParameter);
      arma::Col<double> secondParameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(secondParameter);
      
      CHECK(distanceFunction.getDistance(firstParameter, secondParameter) == Approx(distanceFunction.getLength(secondParameter - firstParameter)));
    }
    
    // TODO Add exception tests
  }

  SECTION(".getLength") {
    SECTION("Returns the length of a vector.") {
      arma::Col<double> parameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameter);
      
      CHECK(distanceFunction.getLength(parameter) == Approx(distanceFunction.getLengthImplementation(parameter)));
    }
  }
}
