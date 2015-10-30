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

class TestGallaghersGaussian101mePeaksFunction : public mant::bbob::GallaghersGaussian101mePeaksFunction {
  public:
    using mant::bbob::GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction;
  
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {
      return mant::bbob::GallaghersGaussian101mePeaksFunction::getObjectiveValueImplementation(parameter);
    }
};

TEST_CASE("bbob::GallaghersGaussian101mePeaksFunction") {
  TestGallaghersGaussian101mePeaksFunction gallaghersGaussian101mePeaksFunction(40);
    
  SECTION(".getObjectiveValueImplementation") {
    SECTION("Returns the objective value") {
      arma::Mat<double> rotationQ;
      REQUIRE(rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_1.input"));
      gallaghersGaussian101mePeaksFunction.setRotationQ(rotationQ);

      arma::Mat<double> conditionings;
      REQUIRE(conditionings.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_conditionings_101x1_dim40.input"));
      gallaghersGaussian101mePeaksFunction.setLocalParameterConditionings(conditionings);

      arma::Mat<double> localOptima;
      REQUIRE(localOptima.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_localOptima_40x101.input"));
      gallaghersGaussian101mePeaksFunction.setLocalParameterTranslations(localOptima);

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_gallaghersGaussian101mePeaksFunction_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(gallaghersGaussian101mePeaksFunction.getObjectiveValueImplementation(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      CHECK(gallaghersGaussian101mePeaksFunction.toString() =="bbob_gallaghers_gaussian_101me_peaks_function");
    }
  }
}
