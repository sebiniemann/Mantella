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
    "KrigingModel") {
  SECTION(
      ".model") {
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns the expected class name.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      CHECK(mant::KrigingModel(optimisationProblem->getCachedSamples(), std::shared_ptr<mant::RegressionFunction>(new mant::LinearRegressionFunction(optimisationProblem->numberOfDimensions_)), std::shared_ptr<mant::CorrelationFunction>(new mant::GaussianCorrelationFunction(optimisationProblem->numberOfDimensions_))).toString() ==
            "kriging_model");
    }
  }
}
