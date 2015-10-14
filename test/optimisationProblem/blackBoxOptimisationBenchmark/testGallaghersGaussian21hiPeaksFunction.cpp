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

class TestGallaghersGaussian21hiPeaksFunction : public mant::bbob::GallaghersGaussian21hiPeaksFunction {
  public:
    using mant::bbob::GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::GallaghersGaussian21hiPeaksFunction::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::GallaghersGaussian21hiPeaksFunction") {
  TestGallaghersGaussian21hiPeaksFunction gallaghersGaussian21hiPeaksFunction(40);
    
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> rotationQ;
      REQUIRE(rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_1.input"));
      gallaghersGaussian21hiPeaksFunction.setRotationQ(rotationQ);

      arma::Mat<double> conditionings;
      REQUIRE(conditionings.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_conditionings_21x1_dim40.input"));
      gallaghersGaussian21hiPeaksFunction.setLocalParameterConditionings(conditionings);

      arma::Mat<double> localOptima;
      REQUIRE(localOptima.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_localOptima_40x21.input"));
      gallaghersGaussian21hiPeaksFunction.setLocalParameterTranslations(localOptima);

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_gallaghersGaussian21hiPeaksFunction_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(gallaghersGaussian21hiPeaksFunction.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(gallaghersGaussian21hiPeaksFunction.toString() =="bbob_gallaghers_gaussian_21hi_peaks_function");
    }
  }
}
