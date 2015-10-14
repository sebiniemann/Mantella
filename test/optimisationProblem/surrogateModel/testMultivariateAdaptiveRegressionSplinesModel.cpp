// Catch
#include <catch.hpp>

// C++ standard library
#include <random>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE(
    "MultivariateAdaptiveRegressionSplinesModel") {
  SECTION(
      ".model") {
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns a (filesystem friendly) name for the class.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      CHECK(mant::MultivariateAdaptiveRegressionSplinesModel(optimisationProblem->getCachedSamples()).toString() ==
            "multivariate_adaptive_regression_splines_model");
    }
  }
}
