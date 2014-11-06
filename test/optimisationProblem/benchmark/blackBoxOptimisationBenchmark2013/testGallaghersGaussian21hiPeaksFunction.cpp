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
  for (auto numberOfDimensions : {2, 40}) {
    hop::bbob2013::GallaghersGaussian21hiPeaksFunction gallaghersGaussian21hiPeaksFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> deltaC21;
    deltaC21.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/deltaC21,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> localOptimaY21;
    localOptimaY21.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/localOptimaY21,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/expectedGallaghersGaussian21hiPeaksFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    // TODO Adjust objectiveValueTranslation
    gallaghersGaussian21hiPeaksFunction.setObjectiveValueTranslation(0);
    gallaghersGaussian21hiPeaksFunction.setRotationR(rotationR);
    gallaghersGaussian21hiPeaksFunction.setDeltaC21(deltaC21);
    gallaghersGaussian21hiPeaksFunction.setLocalOptimaY21(localOptimaY21);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(gallaghersGaussian21hiPeaksFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }
}

