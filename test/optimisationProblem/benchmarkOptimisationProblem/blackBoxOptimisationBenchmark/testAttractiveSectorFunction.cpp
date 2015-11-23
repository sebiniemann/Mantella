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

class TestAttractiveSectorFunction : public mant::bbob::AttractiveSectorFunction {
  public:
    using mant::bbob::AttractiveSectorFunction::AttractiveSectorFunction;
  
    // Increases the visibility of internal parameters, to make them accessible.
    // The attractive sector function depends internally on the parameter translation, wherefore it must also be set.
    using mant::bbob::AttractiveSectorFunction::parameterTranslation_;
    using mant::bbob::AttractiveSectorFunction::rotationQ_;
    
    // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
    using mant::bbob::AttractiveSectorFunction::objectiveFunction_;
};

TEST_CASE("bbob::AttractiveSectorFunction") {
  // All expected objective values where generated for a 40-dimensional problem instance.
  TestAttractiveSectorFunction attractiveSectorFunction(40);
      
  SECTION(".objectiveFunction_") {
    SECTION("Returns the expected objective value") {
      // *Note:* There is no need to *CAPTURE* anything below, as everything is stored on the hard disk, and easily accessible.
      
      REQUIRE(attractiveSectorFunction.parameterTranslation_.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_parameterTranslation_40x1.input"));
      REQUIRE(attractiveSectorFunction.rotationQ_.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_1.input"));

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));
      
      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/attractiveSectorFunction_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(attractiveSectorFunction.objectiveFunction_(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }
}
