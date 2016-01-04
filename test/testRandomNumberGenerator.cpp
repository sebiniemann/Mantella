// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

// The following tests are intently avoiding fuzzy testing, as we are challenging the random generator itself.
SCENARIO("Rng::getGenerator", "[randomNumberGenerator][Rng::getGenerator]") {
  THEN("Return a random number generator for the C++ standard library random functions") {
    arma::Col<double>::fixed<10000> randomValues;
    for (arma::uword n = 0; n < randomValues.n_elem; ++n) {
      randomValues(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
    }
    CAPTURE(randomValues);

    IS_UNIFORM(randomValues, 0, 1);
  }
}

// This needs to be tested for the C++ standard library as well as Armadillo, as Armadillo might use another generator.
SCENARIO("Rng::setSeed", "[randomNumberGenerator][Rng::setSeed]") {
  GIVEN("A seed") {
    WHEN("Resetting the seed to a previous value") {
      const arma::arma_rng::seed_type seed = 12345;
      CAPTURE(seed);

      mant::Rng::setSeed(seed);

      THEN("Return the same random (C++ standard library) sequence") {
        arma::Col<double>::fixed<10> expectedRandomValues;
        for (arma::uword n = 0; n < expectedRandomValues.n_elem; ++n) {
          expectedRandomValues(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
        }
        CAPTURE(expectedRandomValues);

        // Resets the seed
        mant::Rng::setSeed(seed);

        arma::Col<double>::fixed<10> actualRandomValues;
        for (arma::uword n = 0; n < actualRandomValues.n_elem; ++n) {
          actualRandomValues(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
        }
        CAPTURE(actualRandomValues);

        IS_EQUAL(actualRandomValues, expectedRandomValues);
      }

      THEN("Return the same random (Armadillo) sequence") {
        const arma::Col<double>::fixed<10> expectedRandomValues = arma::randu<arma::Col<double>>(10);
        CAPTURE(expectedRandomValues);

        // Resets the seed
        mant::Rng::setSeed(seed);

        const arma::Col<double>::fixed<10> actualRandomValues = arma::randu<arma::Col<double>>(10);
        CAPTURE(actualRandomValues);

        IS_EQUAL(actualRandomValues, expectedRandomValues);
      }
    }
  }
}

SCENARIO("Rng::getSeed", "[randomNumberGenerator][Rng::getSeed]") {
  GIVEN("Default random seed") {
    THEN("Return the default random seed") {
      CHECK(mant::Rng::getSeed() == 12345);
    }
  }

  GIVEN("An updated random seed") {
    const arma::arma_rng::seed_type seed = 12345;
    CAPTURE(seed);

    mant::Rng::setSeed(seed);

    THEN("Return the updated random seed") {
      CHECK(mant::Rng::getSeed() == seed);
    }
  }
}

SCENARIO("Rng::setRandomSeed", "[randomNumberGenerator][Rng::setRandomSeed]") {
  THEN("Set the seed to a uniformly and randomly distributed value") {
    arma::Col<arma::uword>::fixed<10000> seeds;
    for (arma::uword n = 0; n < seeds.n_elem; ++n) {
      mant::Rng::setRandomSeed();
      seeds(n) = mant::Rng::getSeed();
    }
    CAPTURE(seeds);

    IS_UNIFORM(seeds, 0, std::numeric_limits<arma::uword>::max());
  }

#if defined(SUPPORT_MPI)
  WHEN("MPI is supported") {
    THEN("Return a different seed for each node") {
      mant::Rng::setRandomSeed();
      unsigned int seed = static_cast<unsigned int>(mant::Rng::getSeed());
      arma::Col<unsigned int> gatheredSeeds(static_cast<arma::uword>(numberOfNodes));

      MPI_Allgather(&seed, 1, MPI_UNSIGNED, gatheredSeeds.memptr(), 1, MPI_UNSIGNED, MPI_COMM_WORLD);

      arma::Col<arma::uword> seeds = arma::conv_to<arma::Col<arma::uword>>::from(gatheredSeeds);
      CAPTURE(seeds);

      for (arma::uword n = 0; n < seeds.n_elem; ++n) {
        for (arma::uword k = n + 1; k < seeds.n_elem; ++k) {
          CHECK(seeds(n) != seeds(k));
        }
      }
    }
  }
#endif
}
