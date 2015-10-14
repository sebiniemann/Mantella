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

class TestSchaffersF7FunctionIllConditioned : public mant::bbob::SchaffersF7FunctionIllConditioned {
  public:
    using mant::bbob::SchaffersF7FunctionIllConditioned::SchaffersF7FunctionIllConditioned;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::SchaffersF7FunctionIllConditioned::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::SchaffersF7FunctionIllConditioned") {
  TestSchaffersF7FunctionIllConditioned schaffersF7FunctionIllConditioned(40);
    
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> rotationQ;
      REQUIRE(rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_2.input"));
      schaffersF7FunctionIllConditioned.setRotationQ(rotationQ);

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_schaffersF7FunctionIllConditioned_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(schaffersF7FunctionIllConditioned.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(schaffersF7FunctionIllConditioned.toString() =="bbob_schaffers_f7_function_ill_conditioned");
    }
  }
}
