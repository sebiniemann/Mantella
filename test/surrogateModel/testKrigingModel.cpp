// Catch
#include <catch.hpp>

// C++ standard library
#include <random>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("KrigingModel") {
  SECTION(".model") {
  
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::KrigingModel(2).toString() == "kriging_model");
    }
  }
}
