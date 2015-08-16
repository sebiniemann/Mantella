// Catch
#include <catch.hpp>

// C++ Standard Library
#include <memory>

// Mantella
#include <mantella>

TEST_CASE("ParticleSwarmOptimisation", "") {
  SECTION(".optimise") {
    // TODO
  }

  SECTION("Returns the specified class name.") {
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    CHECK(mant::ParticleSwarmOptimisation(optimisationProblem, 10).toString() == "particle_swarm_optimisation");
  }
}