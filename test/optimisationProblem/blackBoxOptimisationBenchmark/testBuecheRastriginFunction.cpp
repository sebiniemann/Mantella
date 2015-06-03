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

TEST_CASE("bbob::BuecheRastriginFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::BuecheRastriginFunction<> buecheRastriginFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/translation,dim" + std::to_string(numberOfDimensions) +".mat");
    for (std::size_t n = 0; n < translation.n_elem; n += 2) {
      translation(n) = std::abs(translation(n));
    }

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/expectedBuecheRastriginFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    buecheRastriginFunction.setObjectiveValueTranslation(0);
    buecheRastriginFunction.setParameterTranslation(translation);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(buecheRastriginFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::BuecheRastriginFunction<>(5).toString() == "bbob_bueche_rastrigin_function");
  }
}
