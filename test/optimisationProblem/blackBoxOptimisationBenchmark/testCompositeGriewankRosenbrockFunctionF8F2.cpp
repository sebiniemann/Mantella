// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

extern std::string testDirectory;

TEST_CASE("bbob::CompositeGriewankRosenbrockFunctionF8F2") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::CompositeGriewankRosenbrockFunctionF8F2 compositeGriewankRosenbrockFunctionF8F2(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) + "x10.input"));

    arma::Mat<double> rotationR;
    REQUIRE(rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) + "x" + std::to_string(numberOfDimensions) + "_2.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_compositeGriewankRosenbrockFunctionF8F2_dim" + std::to_string(numberOfDimensions) +".expected"));

    compositeGriewankRosenbrockFunctionF8F2.setObjectiveValueTranslation(0);
    compositeGriewankRosenbrockFunctionF8F2.setParameterRotation(rotationR);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(compositeGriewankRosenbrockFunctionF8F2.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::bbob::CompositeGriewankRosenbrockFunctionF8F2(5).toString() == "bbob_composite_griewank_rosenbrock_function_f8f2");
    }
  }
}
