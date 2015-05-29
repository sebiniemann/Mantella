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

TEST_CASE("bbob::EllipsoidalFunctionRotated", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::EllipsoidalFunctionRotated<> ellipsoidalFunctionRotated(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/expectedEllipsoidalFunctionRotated,dim" + std::to_string(numberOfDimensions) +".mat");

    ellipsoidalFunctionRotated.setObjectiveValueTranslation(0);
    ellipsoidalFunctionRotated.setParameterTranslation(translation);
    ellipsoidalFunctionRotated.setParameterRotation(rotationR);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(ellipsoidalFunctionRotated.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::EllipsoidalFunctionRotated<>(5).toString() == "bbob_ellipsoidal_function_rotated");
  }
}
