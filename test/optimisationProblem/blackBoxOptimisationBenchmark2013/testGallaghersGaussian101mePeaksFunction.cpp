// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

extern std::string testDirectory;

TEST_CASE("bbob2013::GallaghersGaussian101mePeaksFunction", "") {
//  for (const auto& numberOfDimensions : {2, 40}) {
//    mant::bbob2013::GallaghersGaussian101mePeaksFunction gallaghersGaussian101mePeaksFunction(numberOfDimensions);

//    arma::Mat<double> parameters;
//    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

//    arma::Mat<double> rotationR;
//    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

//    arma::Mat<double> deltaC101;
//    deltaC101.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/deltaC101,dim" + std::to_string(numberOfDimensions) +".mat");

//    arma::Mat<double> localOptimaY101;
//    localOptimaY101.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/localOptimaY101,dim" + std::to_string(numberOfDimensions) +".mat");

//    arma::Col<double> expected;
//    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/expectedGallaghersGaussian101mePeaksFunction,dim" + std::to_string(numberOfDimensions) +".mat");

//    gallaghersGaussian101mePeaksFunction.setObjectiveValueTranslation(0);
//    gallaghersGaussian101mePeaksFunction.setParameterRotationR(rotationR);
//    gallaghersGaussian101mePeaksFunction.setLocalParameterTranslation(localOptimaY101);
//    gallaghersGaussian101mePeaksFunction.setLocalParameterConditioning(deltaC101);

//    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
//      CHECK(gallaghersGaussian101mePeaksFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
//    }
//  }

//  SECTION("Returns the specified class name.") {
//    CHECK(mant::bbob2013::GallaghersGaussian101mePeaksFunction(5).toString() == "gallaghers-gaussian-101me-peaks-function");
//  }
}

