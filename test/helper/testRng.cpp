// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <random>

// Mantella
#include <mantella>

TEST_CASE("Rng") {
  SECTION("Getting a working generator.") {
    arma::Col<double>::fixed<1000> values;
    for (arma::uword n = 0; n < values.n_elem; ++n) {
      values.at(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
    }

    arma::Col<arma::uword> histogram = arma::hist(values, arma::linspace<arma::Col<double>>(0.05, 0.95, 10));
    CHECK(0.25 > static_cast<double>(histogram.max() - histogram.min()) / static_cast<double>(values.n_elem));
  }

  SECTION("Can specify a seed.") {
    arma::uword seed = 12345;
    mant::Rng::setSeed(seed);

    SECTION("Works with the C++ standard library.") {
      arma::Col<double>::fixed<10> expectedValues;
      for (arma::uword n = 0; n < expectedValues.n_elem; ++n) {
        expectedValues.at(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
      }

      mant::Rng::setSeed(seed);
      arma::Col<double>::fixed<10> actualValues;
      for (arma::uword n = 0; n < actualValues.n_elem; ++n) {
        actualValues.at(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
      }

      COMPARE(actualValues, expectedValues);
    }

    SECTION("Works with Armadillo.") {
      arma::Col<double>::fixed<10> expectedValues = arma::randu<arma::Col<double>>(10);
      mant::Rng::setSeed(seed);
      arma::Col<double>::fixed<10> actualValues = arma::randu<arma::Col<double>>(10);

      COMPARE(actualValues, expectedValues);
    }

    // Returns last seed
    CHECK(mant::Rng::getSeed() == seed);
  }

  SECTION("Can use a random seed.") {
    mant::Rng::setRandomSeed();

    SECTION("Works with the C++ standard library.") {
      arma::Col<double>::fixed<1000> values;
      for (arma::uword n = 0; n < values.n_elem; ++n) {
        values.at(n) = std::uniform_real_distribution<double>(0, 1)(mant::Rng::getGenerator());
      }

      arma::Col<arma::uword> histogram = arma::hist(values, arma::linspace<arma::Col<double>>(0.05, 0.95, 10));
      CHECK(0.25 > static_cast<double>(histogram.max() - histogram.min()) / static_cast<double>(values.n_elem));
    }

    SECTION("Works with Armadillo.") {
      arma::Col<double>::fixed<1000> values = arma::randu<arma::Col<double>>(1000);

      arma::Col<arma::uword> histogram = arma::hist(values, arma::linspace<arma::Col<double>>(0.05, 0.95, 10));
      CHECK(0.25 > static_cast<double>(histogram.max() - histogram.min()) / static_cast<double>(values.n_elem));
    }
  }
}
