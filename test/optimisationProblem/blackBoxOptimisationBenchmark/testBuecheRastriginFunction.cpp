// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <string>
#include <cmath>
#include <algorithm>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

extern std::string testDirectory;

class TestBuecheRastriginFunction : public mant::bbob::BuecheRastriginFunction {
  public:
    using mant::bbob::BuecheRastriginFunction::BuecheRastriginFunction;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::BuecheRastriginFunction::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::BuecheRastriginFunction") {
  TestBuecheRastriginFunction buecheRastriginFunction(40);
    
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_buecheRastriginFunction_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(buecheRastriginFunction.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(buecheRastriginFunction.toString() =="bbob_bueche_rastrigin_function");
    }
  }
}
