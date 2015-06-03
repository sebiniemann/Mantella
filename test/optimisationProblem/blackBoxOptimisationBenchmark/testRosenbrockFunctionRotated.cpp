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

TEST_CASE("bbob::RosenbrockFunctionRotated", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::RosenbrockFunctionRotated<> rosenbrockFunctionRotated(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/expectedRosenbrockFunctionRotated,dim" + std::to_string(numberOfDimensions) +".mat");

    rosenbrockFunctionRotated.setObjectiveValueTranslation(0);
    rosenbrockFunctionRotated.setParameterRotation(rotationR);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(rosenbrockFunctionRotated.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::RosenbrockFunctionRotated<>(5).toString() == "bbob_rosenbrock_function_rotated");
  }
}
