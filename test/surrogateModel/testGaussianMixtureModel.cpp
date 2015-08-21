// Catch
#include <catch.hpp>

// C++ standard library
#include <random>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("GaussianMixtureModel") {
  SECTION(".model") {
  
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::GaussianMixtureModel(2).toString() == "gaussian_mixture_model");
    }
  }
}
