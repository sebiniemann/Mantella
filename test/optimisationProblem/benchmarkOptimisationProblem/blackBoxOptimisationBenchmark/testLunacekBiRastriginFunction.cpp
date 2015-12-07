// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

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
  
    // Increases the visibility of internal parameters, to make them accessible.
    using mant::bbob::LunacekBiRastriginFunction::rotationR_;
    using mant::bbob::LunacekBiRastriginFunction::rotationQ_;
    
    // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
    using mant::bbob::LunacekBiRastriginFunction::objectiveFunction_;
};

TEST_CASE("bbob::LunacekBiRastriginFunction") {
  // All expected objective values where generated for a 40-dimensional problem instance.
  TestLunacekBiRastriginFunction optimisationProblem(40);
    
  SECTION(".objectiveFunction_") {
    SECTION("Returns the expected objective value") {
      // *Note:* There is no need to *CAPTURE* anything below, as everything is stored on the hard disk, and easily accessible.
      
      REQUIRE(optimisationProblem.rotationR_.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_1.input"));
      REQUIRE(optimisationProblem.rotationQ_.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_2.input"));

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));
      
      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/lunacekBiRastriginFunction_dim40_1x100.expected"));


      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(optimisationProblem.objectiveFunction_(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }
}
