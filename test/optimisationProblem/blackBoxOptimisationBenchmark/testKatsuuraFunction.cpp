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

class TestKatsuuraFunction : public mant::bbob::KatsuuraFunction {
  public:
    using mant::bbob::KatsuuraFunction::KatsuuraFunction;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::KatsuuraFunction::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::KatsuuraFunction") {
  TestKatsuuraFunction katsuuraFunction(40);
    
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> rotationQ;
      REQUIRE(rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_2.input"));
      katsuuraFunction.setRotationQ(rotationQ);

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));
                             
      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_katsuuraFunction_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(katsuuraFunction.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(katsuuraFunction.toString() =="bbob_katsuura_function");
    }
  }
}
