// Catch
#include <catch.hpp>

// C++ Standard Library
#include <memory>

// Mantella
#include <mantella>

TEST_CASE("HillClimbing", "") {
  SECTION(".optimise") {
    // TODO
  }

  SECTION("Returns the specified class name.") {
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    CHECK(mant::HillClimbing(optimisationProblem).toString() == "hill_climbing");
  }
}