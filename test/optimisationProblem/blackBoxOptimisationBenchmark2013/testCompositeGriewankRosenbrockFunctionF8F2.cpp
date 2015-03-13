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

TEST_CASE("bbob2013::CompositeGriewankRosenbrockFunctionF8F2", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2013::CompositeGriewankRosenbrockFunctionF8F2 compositeGriewankRosenbrockFunctionF8F2(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/expectedCompositeGriewankRosenbrockFunctionF8F2,dim" + std::to_string(numberOfDimensions) +".mat");

    compositeGriewankRosenbrockFunctionF8F2.setObjectiveValueTranslation(0);
    compositeGriewankRosenbrockFunctionF8F2.setParameterRotationR(rotationR);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(compositeGriewankRosenbrockFunctionF8F2.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob2013::CompositeGriewankRosenbrockFunctionF8F2(5).toString() == "composite-griewank-rosenbrock-function-f8f2");
  }
}
