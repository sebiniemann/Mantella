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

TEST_CASE("bbob::LinearSlope") {
  for (const auto& degree : {0, 180}) {
    for (const auto& numberOfDimensions : {2, 40}) {
      mant::bbob::LinearSlope linearSlope(numberOfDimensions);

      arma::Mat<double> parameters;
      REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) + "x10.input"));

      arma::Mat<double> one;
      REQUIRE(one.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_one_" + std::to_string(numberOfDimensions) +"x1.input"));

      arma::Mat<double> rotation;
      REQUIRE(rotation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_rotationsMatrix_" + std::to_string(numberOfDimensions) + "x" + std::to_string(numberOfDimensions) + "_deg" + std::to_string(degree) + ".input"));

      arma::Col<double> expected;
      REQUIRE(expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_linearSlope_dim" + std::to_string(numberOfDimensions) + "deg" + std::to_string(degree) + ".expected"));

      linearSlope.setObjectiveValueTranslation(0);
      linearSlope.setParameterRotation(rotation);

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(linearSlope.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
      }
    }
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::bbob::LinearSlope(5).toString() == "bbob_linear_slope");
    }
  }
}
