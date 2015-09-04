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

TEST_CASE("bbob::GallaghersGaussian21hiPeaksFunction") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::GallaghersGaussian21hiPeaksFunction gallaghersGaussian21hiPeaksFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) + "x10.input"));

    arma::Mat<double> rotationR;
    REQUIRE(rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) + "x" + std::to_string(numberOfDimensions) + "_2.input"));

    arma::Mat<double> conditionings;
    REQUIRE(conditionings.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_conditionings_21x1_dim" + std::to_string(numberOfDimensions) +".input"));

    arma::Mat<double> localOptima;
    REQUIRE(localOptima.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_localOptima_" + std::to_string(numberOfDimensions) +"x21.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_gallaghersGaussian21hiPeaksFunction_dim" + std::to_string(numberOfDimensions) +".expected"));

    gallaghersGaussian21hiPeaksFunction.setObjectiveValueTranslation(0);
    gallaghersGaussian21hiPeaksFunction.setRotationQ(rotationR);
    gallaghersGaussian21hiPeaksFunction.setLocalParameterTranslations(localOptima);
    gallaghersGaussian21hiPeaksFunction.setLocalParameterConditionings(conditionings);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(gallaghersGaussian21hiPeaksFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::bbob::GallaghersGaussian21hiPeaksFunction(5).toString() == "bbob_gallaghers_gaussian_21hi_peaks_function");
    }
  }
}

