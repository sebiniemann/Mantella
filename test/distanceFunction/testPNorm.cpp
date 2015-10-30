// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

class TestPNorm : public mant::PNorm {
  public:
    using mant::PNorm::PNorm;
    
    double getLengthImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::PNorm::getLengthImplementation(parameter);
    }
};

TEST_CASE("PNorm") {
  const arma::uword numberOfDimensions = getRandomNumberOfValues();
  CAPTURE(numberOfDimensions);
      
  const arma::uword p = getRandomNumberOfValues(1);
  CAPTURE(p);

  TestPNorm pNorm(numberOfDimensions, p);

  SECTION(".p_") {
    SECTION("P is public accessible and set by the constructor.") {
      CHECK(pNorm.p_ == p);
    }
  }

  SECTION(".getLengthImplementation") {
    SECTION("Returns the p-norm-based length of a vector.") {
      arma::Col<double> parameter = getRandomValues(numberOfDimensions, 1);
      CAPTURE(parameter);
      
      CHECK(pNorm.getLengthImplementation(parameter) == Approx(arma::norm(parameter, p)));
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(pNorm.toString() == std::to_string(p) + "_norm");
    }
  }
  
    // TODO Add exception tests (p >= 1)
}
