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

TEST_CASE("bbob2010::SchaffersF7FunctionIllConditioned", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2010::SchaffersF7FunctionIllConditioned schaffersF7FunctionIllConditioned(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationQ;
    rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationQ,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/expectedSchaffersF7FunctionIllConditioned,dim" + std::to_string(numberOfDimensions) +".mat");

    schaffersF7FunctionIllConditioned.setObjectiveValueTranslation(0);
    schaffersF7FunctionIllConditioned.setLocalParameterTranslation(translation);
    schaffersF7FunctionIllConditioned.setRotationR(rotationR);
    schaffersF7FunctionIllConditioned.setRotationQ(rotationQ);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(schaffersF7FunctionIllConditioned.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob2010::SchaffersF7FunctionIllConditioned(5).toString() == "schaffers-f7-function-ill-conditioned");
  }
}
