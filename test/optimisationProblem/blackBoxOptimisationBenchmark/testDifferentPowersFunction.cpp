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

TEST_CASE("bbob::DifferentPowersFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::DifferentPowersFunction<> differentPowersFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/expectedDifferentPowersFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    differentPowersFunction.setObjectiveValueTranslation(0);
    differentPowersFunction.setParameterTranslation(translation);
    differentPowersFunction.setParameterRotation(rotationR);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(differentPowersFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::DifferentPowersFunction<>(5).toString() == "bbob_different_powers_function");
  }
}
