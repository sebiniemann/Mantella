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
    "bbob::EllipsoidalFunction") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::EllipsoidalFunction ellipsoidalFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory +
                            "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) +
                            "x10.input"));

    arma::Col<double> translation;
    REQUIRE(translation.load(testDirectory +
                             "/data/optimisationProblem/blackBoxOptimisationBenchmark/_translation_" + std::to_string(numberOfDimensions) +
                             "x1.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory +
                          "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_ellipsoidalFunction_dim" + std::to_string(numberOfDimensions) +
                          ".expected"));

    ellipsoidalFunction.setObjectiveValueTranslation(0);
    ellipsoidalFunction.setParameterTranslation(translation);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(ellipsoidalFunction.getObjectiveValue(parameters.col(n)) == Approx(expected(n)));
    }
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns the expected class name.") {
      CHECK(mant::bbob::EllipsoidalFunction(5).toString() ==
            "bbob_ellipsoidal_function");
    }
  }
}
