// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

extern std::string testDirectory;

class TestRosenbrockFunctionRotated : public mant::bbob::RosenbrockFunctionRotated {
  public:
    using mant::bbob::RosenbrockFunctionRotated::RosenbrockFunctionRotated;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::RosenbrockFunctionRotated::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::RosenbrockFunctionRotated") {
  TestRosenbrockFunctionRotated rosenbrockFunctionRotated(40);
    
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_rosenbrockFunctionRotated_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(rosenbrockFunctionRotated.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(rosenbrockFunctionRotated.toString() =="bbob_rosenbrock_function_rotated");
    }
  }
}
