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

TEST_CASE("BBOB2015-SchwefelFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2015::SchwefelFunction schwefelFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> one;
    one.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/one,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/expectedSchwefelFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    schwefelFunction.setObjectiveValueTranslation(0);
    schwefelFunction.setOne(one);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(schwefelFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }
}
