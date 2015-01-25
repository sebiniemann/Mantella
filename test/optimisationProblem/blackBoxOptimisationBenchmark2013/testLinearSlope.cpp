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

TEST_CASE("bbob2013::LinearSlope", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2013::LinearSlope linearSlope(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> one;
    one.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/one,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/expectedLinearSlope,dim" + std::to_string(numberOfDimensions) +".mat");

    linearSlope.setObjectiveValueTranslation(0);
    linearSlope.setOne(one);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(linearSlope.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob2013::LinearSlope(5).to_string() == "LinearSlope");
  }
}
