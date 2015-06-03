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

TEST_CASE("bbob::CompositeGriewankRosenbrockFunctionF8F2", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::CompositeGriewankRosenbrockFunctionF8F2<> compositeGriewankRosenbrockFunctionF8F2(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/expectedCompositeGriewankRosenbrockFunctionF8F2,dim" + std::to_string(numberOfDimensions) +".mat");

    compositeGriewankRosenbrockFunctionF8F2.setObjectiveValueTranslation(0);
    compositeGriewankRosenbrockFunctionF8F2.setParameterRotation(rotationR);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(compositeGriewankRosenbrockFunctionF8F2.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::CompositeGriewankRosenbrockFunctionF8F2<>(5).toString() == "bbob_composite_griewank_rosenbrock_function_f8f2");
  }
}
