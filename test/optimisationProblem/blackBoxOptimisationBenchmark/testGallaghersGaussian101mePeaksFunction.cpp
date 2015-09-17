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

TEST_CASE(
    "bbob::GallaghersGaussian101mePeaksFunction") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::GallaghersGaussian101mePeaksFunction gallaghersGaussian101mePeaksFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory +
                            "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) +
                            "x10.input"));

    arma::Mat<double> rotationR;
    REQUIRE(rotationR.load(testDirectory +
                           "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) +
                           "x" + std::to_string(numberOfDimensions) +
                           "_2.input"));

    arma::Mat<double> conditionings;
    REQUIRE(conditionings.load(testDirectory +
                               "/data/optimisationProblem/blackBoxOptimisationBenchmark/_conditionings_101x1_dim" + std::to_string(numberOfDimensions) +
                               ".input"));

    arma::Mat<double> localOptima;
    REQUIRE(localOptima.load(testDirectory +
                             "/data/optimisationProblem/blackBoxOptimisationBenchmark/_localOptima_" + std::to_string(numberOfDimensions) +
                             "x101.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory +
                          "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_gallaghersGaussian101mePeaksFunction_dim" + std::to_string(numberOfDimensions) +
                          ".expected"));

    gallaghersGaussian101mePeaksFunction.setObjectiveValueTranslation(0);
    gallaghersGaussian101mePeaksFunction.setRotationQ(rotationR);
    gallaghersGaussian101mePeaksFunction.setLocalParameterTranslations(localOptima);
    gallaghersGaussian101mePeaksFunction.setLocalParameterConditionings(conditionings);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(gallaghersGaussian101mePeaksFunction.getObjectiveValue(parameters.col(n)) == Approx(expected(n)));
    }
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns the expected class name.") {
      CHECK(mant::bbob::GallaghersGaussian101mePeaksFunction(5).toString() ==
            "bbob_gallaghers_gaussian_101me_peaks_function");
    }
  }
}
