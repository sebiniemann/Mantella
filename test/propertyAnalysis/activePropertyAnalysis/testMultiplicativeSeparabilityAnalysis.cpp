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
    "MultiplicativeSeparabilityAnalysis") {
  SECTION(
      ".analyse") {
    SECTION(
        "Checking the procedure.") {
    }
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns a (filesystem friendly) name for the class.") {
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      CHECK(mant::MultiplicativeSeparabilityAnalysis(optimisationProblem).toString() ==
            "multiplicative_separability_analysis");
    }
  }
}
