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
    "QuadraticProportionalityAnalysis") {
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
      CHECK(mant::PolynomialProportionalityAnalysis(optimisationProblem, std::shared_ptr<mant::RegressionFunction>(new mant::LinearRegressionFunction(optimisationProblem->numberOfDimensions_))).toString() ==
            "polynomial_proportionality_analysis");
    }
  }
}
