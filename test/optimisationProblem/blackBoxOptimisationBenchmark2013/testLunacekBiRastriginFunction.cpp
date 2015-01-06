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

TEST_CASE("LunacekBiRastriginFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    hop::bbob2013::LunacekBiRastriginFunction lunacekBiRastriginFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> one;
    one.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/one,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationQ;
    rotationQ.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationQ,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/expectedLunacekBiRastriginFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    lunacekBiRastriginFunction.setObjectiveValueTranslation(0);
    lunacekBiRastriginFunction.setOne(one);
    lunacekBiRastriginFunction.setRotationR(rotationR);
    lunacekBiRastriginFunction.setRotationQ(rotationQ);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(lunacekBiRastriginFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }
}
