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

TEST_CASE("bbob2009::DifferentPowersFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2009::DifferentPowersFunction differentPowersFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/expectedDifferentPowersFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    differentPowersFunction.setObjectiveValueTranslation(0);
    differentPowersFunction.setLocalParameterTranslation(translation);
    differentPowersFunction.setRotationR(rotationR);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(differentPowersFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob2009::DifferentPowersFunction(5).toString() == "different-powers-function");
  }
}
