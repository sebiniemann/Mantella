// Catch
#include <catch.hpp>

// C++ standard library
#include <random>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("MultivariateAdaptiveRegressionSplinesModel") {
  SECTION(".model") {
  
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::MultivariateAdaptiveRegressionSplinesModel(2).toString() == "multivariate_adaptive_regression_splines_model");
    }
  }
}
