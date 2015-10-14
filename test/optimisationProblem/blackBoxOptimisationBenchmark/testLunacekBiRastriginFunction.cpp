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

class TestLunacekBiRastriginFunction : public mant::bbob::LunacekBiRastriginFunction {
  public:
    using mant::bbob::LunacekBiRastriginFunction::LunacekBiRastriginFunction;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::LunacekBiRastriginFunction::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::LunacekBiRastriginFunction") {
  TestLunacekBiRastriginFunction lunacekBiRastriginFunction(40);
    
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> rotationR;
      REQUIRE(rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_1.input"));
      lunacekBiRastriginFunction.setRotationR(rotationR);

      arma::Mat<double> rotationQ;
      REQUIRE(rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_2.input"));
      lunacekBiRastriginFunction.setRotationQ(rotationQ);

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));
      
      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_lunacekBiRastriginFunction_dim40_1x100.expected"));


      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(lunacekBiRastriginFunction.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(lunacekBiRastriginFunction.toString() =="bbob_lunacek_bi_rastrigin_function");
    }
  }
}
