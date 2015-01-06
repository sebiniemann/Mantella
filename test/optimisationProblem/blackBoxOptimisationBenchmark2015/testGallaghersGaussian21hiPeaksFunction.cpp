// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Boost
#include <boost/filesystem.hpp>

// HOP
#include <hop>

extern boost::filesystem::path testDirectory;

TEST_CASE("GallaghersGaussian21hiPeaksFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    hop::bbob2015::GallaghersGaussian21hiPeaksFunction gallaghersGaussian21hiPeaksFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> deltaC21;
    deltaC21.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/deltaC21,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> localOptimaY21;
    localOptimaY21.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/localOptimaY21,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/expectedGallaghersGaussian21hiPeaksFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    gallaghersGaussian21hiPeaksFunction.setObjectiveValueTranslation(0);
    gallaghersGaussian21hiPeaksFunction.setRotationR(rotationR);
    gallaghersGaussian21hiPeaksFunction.setDeltaC21(deltaC21);
    gallaghersGaussian21hiPeaksFunction.setLocalOptimaY21(localOptimaY21);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(gallaghersGaussian21hiPeaksFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }
}

