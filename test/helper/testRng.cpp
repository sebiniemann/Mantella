// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <random>

// Mantella
#include <mantella>

// This test is intently avoiding fuzzy testing, as we are challenging the random generator itself.
TEST_CASE("Rng") {
  SECTION("::getGenerator") {
    SECTION("Is a valid (working) generator for C++ standard library random functions.") {
      arma::Col<double>::fixed<10000> randomValues;
      for (arma::uword n = 0; n < randomValues.n_elem; ++n) {
        randomValues(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
      }
      CAPTURE(randomValues);

      IS_UNIFORM(randomValues, 0, 1);
    }
  }

  SECTION("::setSeed") {
    const arma::arma_rng::seed_type seed = 12345;
    CAPTURE(seed);
    mant::Rng::setSeed(seed);

    SECTION("Resetting the seed generates the same random sequence.") {
      SECTION("Works with C++ standard library random functions.") {
        arma::Col<double>::fixed<10> expectedRandomValues;
        for (arma::uword n = 0; n < expectedRandomValues.n_elem; ++n) {
          expectedRandomValues(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
        }
        CAPTURE(expectedRandomValues);

        // Reset the seed
        mant::Rng::setSeed(seed);

        arma::Col<double>::fixed<10> actualRandomValues;
        for (arma::uword n = 0; n < actualRandomValues.n_elem; ++n) {
          actualRandomValues(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
        }
        CAPTURE(actualRandomValues);

        IS_EQUAL(actualRandomValues, expectedRandomValues);
      }

      SECTION("Works with Armadillo C++.") {
        const arma::Col<double>::fixed<10> expectedRandomValues = arma::randu<arma::Col<double>>(10);
        CAPTURE(expectedRandomValues);

        // Reset the seed
        mant::Rng::setSeed(seed);

        const arma::Col<double>::fixed<10> actualRandomValues = arma::randu<arma::Col<double>>(10);
        CAPTURE(actualRandomValues);

        IS_EQUAL(actualRandomValues, expectedRandomValues);
      }
    }
  }

  SECTION("::getSeed") {
    SECTION("Returns the current seed.") {
      const arma::arma_rng::seed_type seed = 12345;
      CAPTURE(seed);
      mant::Rng::setSeed(seed);

      // Generate some random values
      arma::Col<double>::fixed<100> randomValues(arma::fill::randu);
      CAPTURE(randomValues);

      CHECK(mant::Rng::getSeed() == seed);
    }
  }

  SECTION("::setRandomSeed") {
    SECTION("The values of a random sequences are uniformly distributed between different random seeds.") {
      arma::Mat<double>::fixed<10000, 10> randomValues;
      for (arma::uword n = 0; n < randomValues.n_rows; ++n) {
        mant::Rng::setRandomSeed();
        CAPTURE(mant::Rng::getSeed());

        randomValues.row(n) = arma::randu<arma::Row<double>>(randomValues.n_cols);
      }
      
      for (arma::uword n = 0; n < randomValues.n_cols; ++n) {
        CAPTURE(randomValues.col(n));
        IS_UNIFORM(randomValues.col(n), 0, 1);
      }
    }
  }
}
