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

TEST_CASE("GallaghersGaussian101mePeaksFunction", "") {
  for (auto numberOfDimensions : {2, 40}) {
    hop::bbob2013::GallaghersGaussian101mePeaksFunction gallaghersGaussian101mePeaksFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> deltaC101;
    deltaC101.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/deltaC101,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> localOptimaY101;
    localOptimaY101.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/localOptimaY101,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/expectedGallaghersGaussian101mePeaksFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    // TODO Adjust objectiveValueTranslation
    gallaghersGaussian101mePeaksFunction.setObjectiveValueTranslation(0);
    gallaghersGaussian101mePeaksFunction.setRotationR(rotationR);
    gallaghersGaussian101mePeaksFunction.setDeltaC101(deltaC101);
    gallaghersGaussian101mePeaksFunction.setLocalOptimaY101(localOptimaY101);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(gallaghersGaussian101mePeaksFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }
}

