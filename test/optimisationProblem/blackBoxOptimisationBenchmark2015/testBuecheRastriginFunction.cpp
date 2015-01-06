// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Boost
#include <boost/filesystem.hpp>

// HOP
#include <hop>

extern boost::filesystem::path testDirectory;

TEST_CASE("BuecheRastriginFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    hop::bbob2015::BuecheRastriginFunction buecheRastriginFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/expectedBuecheRastriginFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    buecheRastriginFunction.setObjectiveValueTranslation(0);
    buecheRastriginFunction.setTranslation(translation);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(buecheRastriginFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }
}
