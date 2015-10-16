// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

extern std::string testDirectory;

class TestDifferentPowersFunction : public mant::bbob::DifferentPowersFunction {
  public:
    using mant::bbob::DifferentPowersFunction::DifferentPowersFunction;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::DifferentPowersFunction::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::DifferentPowersFunction") {
  TestDifferentPowersFunction differentPowersFunction(40);
    
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_differentPowersFunction_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(differentPowersFunction.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(differentPowersFunction.toString() =="bbob_different_powers_function");
    }
  }
}
