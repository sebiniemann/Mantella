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

class TestCompositeGriewankRosenbrockFunctionF8F2 : public mant::bbob::CompositeGriewankRosenbrockFunctionF8F2 {
  public:
    using mant::bbob::CompositeGriewankRosenbrockFunctionF8F2::CompositeGriewankRosenbrockFunctionF8F2;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::CompositeGriewankRosenbrockFunctionF8F2::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::CompositeGriewankRosenbrockFunctionF8F2") {
  TestCompositeGriewankRosenbrockFunctionF8F2 compositeGriewankRosenbrockFunctionF8F2(40);
      
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_compositeGriewankRosenbrockFunctionF8F2_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(compositeGriewankRosenbrockFunctionF8F2.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }
  
  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(compositeGriewankRosenbrockFunctionF8F2.toString() =="bbob_composite_griewank_rosenbrock_function_f8f2");
    }
  }
}
