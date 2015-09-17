// Catch
#include <catch.hpp>

// C++ standard library
#include <memory>
#include <random>

// Mantella
#include <mantella>

TEST_CASE(
    "cacheSamples") {
  SECTION(
      "Is set to true per default") {
    CHECK(mant::cacheSamples == true);
  }

  SECTION(
      "Activates the sample caching") {
    mant::cacheSamples = true;

    const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfDimensions);

    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(numberOfDimensions));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();

    CHECK(optimisationProblem->getCachedSamples().size() > 0);
  }

  SECTION(
      "Deactivates the sample caching") {
    mant::cacheSamples = false;

    const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfDimensions);

    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(numberOfDimensions));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();

    CHECK(optimisationProblem->getCachedSamples().size() == 0);
  }
}

TEST_CASE(
    "recordSamples") {
  SECTION(
      "Is set to false per default") {
    CHECK(mant::recordSamples == false);
  }

  SECTION(
      "Activates the sample recording") {
    mant::recordSamples = true;

    const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfDimensions);

    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(numberOfDimensions));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();

    CHECK(optimisationAlgorithm.getSamplingHistory().size() > 0);
  }

  SECTION(
      "Deactivates the sample recording") {
    mant::recordSamples = false;

    const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfDimensions);

    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(numberOfDimensions));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();

    CHECK(optimisationAlgorithm.getSamplingHistory().size() == 0);
  }
}
