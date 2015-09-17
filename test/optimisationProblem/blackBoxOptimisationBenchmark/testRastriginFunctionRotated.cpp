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

TEST_CASE(
    "bbob::RastriginFunctionRotated") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::RastriginFunctionRotated rastriginFunctionRotated(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory +
                            "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) +
                            "x10.input"));

    arma::Col<double> translation;
    REQUIRE(translation.load(testDirectory +
                             "/data/optimisationProblem/blackBoxOptimisationBenchmark/_translation_" + std::to_string(numberOfDimensions) +
                             "x1.input"));

    arma::Mat<double> rotationR;
    REQUIRE(rotationR.load(testDirectory +
                           "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) +
                           "x" + std::to_string(numberOfDimensions) +
                           "_2.input"));

    arma::Mat<double> rotationQ;
    REQUIRE(rotationQ.load(testDirectory +
                           "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) +
                           "x" + std::to_string(numberOfDimensions) +
                           "_1.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory +
                          "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_rastriginFunctionRotated_dim" + std::to_string(numberOfDimensions) +
                          ".expected"));

    rastriginFunctionRotated.setObjectiveValueTranslation(0);
    rastriginFunctionRotated.setParameterTranslation(translation);
    rastriginFunctionRotated.setRotationR(rotationR);
    rastriginFunctionRotated.setRotationQ(rotationQ);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(rastriginFunctionRotated.getObjectiveValue(parameters.col(n)) == Approx(expected(n)));
    }
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns the expected class name.") {
      CHECK(mant::bbob::RastriginFunctionRotated(5).toString() ==
            "bbob_rastrigin_function_rotated");
    }
  }
}
