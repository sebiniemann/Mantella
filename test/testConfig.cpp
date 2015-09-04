// Catch
#include <catch.hpp>

// C++ standard library
#include <memory>
#include <random>

// Mantella
#include <mantella>

TEST_CASE("cacheSamples") {
  SECTION("Is set to true per default") {
    CHECK(mant::cacheSamples == true);
  }
  
  SECTION("Activates the sample caching") {
    mant::cacheSamples = true;
    
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();
    
    CHECK(optimisationProblem->getCachedSamples().size() > 0);
  }
  
  SECTION("Deactivates the sample caching") {
    mant::cacheSamples = false;
    
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();
    
    CHECK(optimisationProblem->getCachedSamples().size() == 0);
  }
}

TEST_CASE("recordSamples") {
  SECTION("Is set to false per default") {
    CHECK(mant::recordSamples == false);
  }
  
  SECTION("Activates the sample recording") {
    mant::recordSamples = true;
    
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();
    
    CHECK(optimisationAlgorithm.getSamplingHistory().size() > 0);
  }
  
  SECTION("Deactivates the sample recording") {
    mant::recordSamples = false;
    
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();
    
    CHECK(optimisationAlgorithm.getSamplingHistory().size() == 0);
  }
}
