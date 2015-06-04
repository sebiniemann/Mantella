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

TEST_CASE("bbob::LinearSlope", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::LinearSlope<> linearSlope(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> one;
    one.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/one,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/expectedLinearSlope,dim" + std::to_string(numberOfDimensions) +".mat");

    linearSlope.setObjectiveValueTranslation(0);
    linearSlope.setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions, numberOfDimensions) * (one(0) > 0 ? 1.0 : -1.0));

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(linearSlope.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::LinearSlope<>(5).toString() == "bbob_linear_slope");
  }
}
