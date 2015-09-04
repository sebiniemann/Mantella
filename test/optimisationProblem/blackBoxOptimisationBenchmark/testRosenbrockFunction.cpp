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

TEST_CASE("bbob::RosenbrockFunction") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::RosenbrockFunction rosenbrockFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) + "x10.input"));

    arma::Col<double> translation;
    REQUIRE(translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_translation_" + std::to_string(numberOfDimensions) + "x1.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_rosenbrockFunction_dim" + std::to_string(numberOfDimensions) +".expected"));

    rosenbrockFunction.setObjectiveValueTranslation(0);
    rosenbrockFunction.setParameterTranslation(translation);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(rosenbrockFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::bbob::RosenbrockFunction(5).toString() == "bbob_rosenbrock_function");
    }
  }
}
