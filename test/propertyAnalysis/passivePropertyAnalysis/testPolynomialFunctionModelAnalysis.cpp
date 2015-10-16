// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ Standard Library
#include <cstdlib>
#include <random>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE(
    "PolynomialFunctionModelAnalysis") {
  SECTION(
      ".analyse") {
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns a (filesystem friendly) name for the class.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      CHECK(mant::PolynomialFunctionModelAnalysis(optimisationProblem->getCachedSamples(), std::shared_ptr<mant::RegressionFunction>(new mant::LinearRegressionFunction(optimisationProblem->numberOfDimensions_))).toString() ==
            "polynomial_function_model_analysis");
    }
  }
}
