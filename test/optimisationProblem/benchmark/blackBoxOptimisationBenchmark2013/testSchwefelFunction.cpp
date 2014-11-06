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

TEST_CASE("SchwefelFunction", "") {
  for(auto numberOfDimensions : {2, 40}) {
    hop::bbob2013::SchwefelFunction schwefelFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> one;
    one.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/one,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/expectedSchwefelFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    // TODO Adjust objectiveValueTranslation
    schwefelFunction.setObjectiveValueTranslation(0);
    schwefelFunction.setOne(one);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(schwefelFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }
}
