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

TEST_CASE("BBOB2012-RosenbrockFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2013::RosenbrockFunction rosenbrockFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationQ;
    rotationQ.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationQ,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/expectedRosenbrockFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    rosenbrockFunction.setObjectiveValueTranslation(0);
    rosenbrockFunction.setTranslation(translation);
    rosenbrockFunction.setRotationR(rotationR);
    rosenbrockFunction.setRotationQ(rotationQ);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(rosenbrockFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }
}
