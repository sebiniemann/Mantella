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

class TestRastriginFunctionRotated : public mant::bbob::RastriginFunctionRotated {
  public:
    using mant::bbob::RastriginFunctionRotated::RastriginFunctionRotated;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::RastriginFunctionRotated::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::RastriginFunctionRotated") {
  TestRastriginFunctionRotated rastriginFunctionRotated(40);
    
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> rotationR;
      REQUIRE(rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_1.input"));
      rastriginFunctionRotated.setRotationR(rotationR);

      arma::Mat<double> rotationQ;
      REQUIRE(rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_2.input"));
      rastriginFunctionRotated.setRotationQ(rotationQ);

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_rastriginFunctionRotated_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(rastriginFunctionRotated.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(rastriginFunctionRotated.toString() =="bbob_rastrigin_function_rotated");
    }
  }
}
