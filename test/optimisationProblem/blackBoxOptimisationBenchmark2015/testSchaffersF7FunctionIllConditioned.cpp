// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Boost
#include <boost/filesystem.hpp>

// Mantella
#include <mantella>

extern boost::filesystem::path testDirectory;

TEST_CASE("BBOB2015-SchaffersF7FunctionIllConditioned", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2015::SchaffersF7FunctionIllConditioned schaffersF7FunctionIllConditioned(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationQ;
    rotationQ.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/rotationQ,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/expectedSchaffersF7FunctionIllConditioned,dim" + std::to_string(numberOfDimensions) +".mat");

    schaffersF7FunctionIllConditioned.setObjectiveValueTranslation(0);
    schaffersF7FunctionIllConditioned.setTranslation(translation);
    schaffersF7FunctionIllConditioned.setRotationR(rotationR);
    schaffersF7FunctionIllConditioned.setRotationQ(rotationQ);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(schaffersF7FunctionIllConditioned.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }
}
