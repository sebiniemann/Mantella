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

class TestAttractiveSectorFunction : public mant::bbob::AttractiveSectorFunction {
  public:
    using mant::bbob::AttractiveSectorFunction::AttractiveSectorFunction;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::AttractiveSectorFunction::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::AttractiveSectorFunction") {
  TestAttractiveSectorFunction attractiveSectorFunction(40);
      
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      // The attractive sector function depends internally on the parameter translation, wherefore it must be set explicitly.
      // Otherwise, the actual objective values will almost never match the expected ones.
      arma::Mat<double> parameterTranslation;
      REQUIRE(parameterTranslation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_translation_40x1.input"));
      attractiveSectorFunction.setParameterTranslation(parameterTranslation);
      
      arma::Mat<double> rotationQ;
      REQUIRE(rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_1.input"));
      attractiveSectorFunction.setRotationQ(rotationQ);

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));
      
      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_attractiveSectorFunction_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(attractiveSectorFunction.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(attractiveSectorFunction.toString() == "bbob_attractive_sector_function");
    }
  }
}
