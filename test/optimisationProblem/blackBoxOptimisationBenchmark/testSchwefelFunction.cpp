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

TEST_CASE("bbob::SchwefelFunction") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::SchwefelFunction schwefelFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) + "x10.input"));

    arma::Col<double> one;
    REQUIRE(one.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_one_" + std::to_string(numberOfDimensions) +"x1.input"));

    arma::Col<double> scaling;
    REQUIRE(scaling.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_scaling_" + std::to_string(numberOfDimensions) +"x1.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_schwefelFunction_dim" + std::to_string(numberOfDimensions) +".expected"));

    schwefelFunction.setObjectiveValueTranslation(0);
    schwefelFunction.setParameterScaling(scaling);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(schwefelFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::bbob::SchwefelFunction(5).toString() == "bbob_schwefel_function");
    }
  }
}
