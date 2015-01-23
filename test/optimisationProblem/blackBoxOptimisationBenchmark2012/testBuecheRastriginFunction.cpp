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

TEST_CASE("BBOB2012-BuecheRastriginFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2013::BuecheRastriginFunction buecheRastriginFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/expectedBuecheRastriginFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    buecheRastriginFunction.setObjectiveValueTranslation(0);
    buecheRastriginFunction.setTranslation(translation);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(buecheRastriginFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob2009::AttractiveSectorFunction(5).to_string() == "BuecheRastriginFunction");
  }
}
