// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <memory>
#include <random>

// Mantella
#include <mantella>

TEST_CASE("cacheSamples") {
  const arma::uword numberOfDimensions = getRandomNumberOfValues();
  CAPTURE(numberOfDimensions);
  
  std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(numberOfDimensions));
  mant::RandomSearch optimisationAlgorithm(optimisationProblem);
  
  SECTION("Is set to true per default") {
    CHECK(mant::cacheSamples == true);
  }
  
  SECTION("Activates the sample caching") {
    mant::cacheSamples = true;
    optimisationAlgorithm.optimise();
    CHECK(optimisationProblem->getCachedSamples().size() > 0);
  }

  SECTION("Deactivates the sample caching") {
    mant::cacheSamples = false;
    optimisationAlgorithm.optimise();
    CHECK(optimisationProblem->getCachedSamples().size() == 0);
  }
}

TEST_CASE("recordSamples") {
  const arma::uword numberOfDimensions = getRandomNumberOfValues();
  CAPTURE(numberOfDimensions);

  std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(numberOfDimensions));
  mant::RandomSearch optimisationAlgorithm(optimisationProblem);
  
  SECTION("Is set to false per default") {
    CHECK(mant::recordSamples == false);
  }

  SECTION("Activates the sample recording") {
    mant::recordSamples = true;
    optimisationAlgorithm.optimise();
    CHECK(optimisationAlgorithm.getSamplingHistory().size() > 0);
  }

  SECTION("Deactivates the sample recording") {
    mant::recordSamples = false;
    optimisationAlgorithm.optimise();
    CHECK(optimisationAlgorithm.getSamplingHistory().size() == 0);
  }
}
