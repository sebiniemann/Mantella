// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE(
    "TranslationPeriodicityAnalysis") {
  SECTION(
      ".analyse") {
    SECTION(
        "Checking the procedure.") {
    }
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns the expected class name.") {
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      CHECK(mant::TranslationPeriodicityAnalysis(optimisationProblem).toString() ==
            "translation_periodicity_analysis");
    }
  }
}
