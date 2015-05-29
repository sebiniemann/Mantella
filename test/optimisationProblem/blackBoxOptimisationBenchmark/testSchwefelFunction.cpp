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

TEST_CASE("bbob::SchwefelFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::SchwefelFunction<> schwefelFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> one;
    one.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/one,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/expectedSchwefelFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    schwefelFunction.setObjectiveValueTranslation(0);
    schwefelFunction.setParameterScaling(arma::zeros<arma::Col<double>>(numberOfDimensions) + (one.at(0) > 0 ? 2.0 : -2.0));

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(schwefelFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::SchwefelFunction<>(5).toString() == "bbob_schwefel_function");
  }
}
