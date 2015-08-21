// Catch
#include <catch.hpp>

// C++ standard library
#include <random>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("RadialBasisFunctionModel") {
  SECTION(".model") {
  
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::RadialBasisFunctionModel(2).toString() == "radial_basis_function_model");
    }
  }
}
