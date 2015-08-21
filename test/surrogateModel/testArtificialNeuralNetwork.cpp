// Catch
#include <catch.hpp>

// C++ standard library
#include <random>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("ArtificialNeuralNetwork") {
  SECTION(".model") {
  
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::ArtificialNeuralNetwork(2).toString() == "artificial_neural_network");
    }
  }
}
