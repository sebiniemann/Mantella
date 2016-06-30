// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("Rng::setSeed", "[randomNumberGenerator][Rng::setSeed]") {
  GIVEN("A seed") {
    WHEN("The seed is reset after generating a sequence of random numbers") {
      THEN("Return the same random values again") {
        mant::Rng::setSeed(12345);

        // Tests Armadillo's generator and `mant::Rng::generator_` as both are usually independent.
        arma::uvec::fixed<10> cppStandardLibraryRandomValues;
        std::uniform_int_distribution<arma::uword> uniformIntegerDistribution;
        for (arma::uword n = 0; n < cppStandardLibraryRandomValues.n_elem; ++n) {
          cppStandardLibraryRandomValues(n) = uniformIntegerDistribution(mant::Rng::generator_);
        }
        const arma::uvec::fixed<10> armadilloRandomValues = arma::randi<arma::uvec>(10);

        mant::Rng::setSeed(12345);

        for (arma::uword n = 0; n < cppStandardLibraryRandomValues.n_elem; ++n) {
          CHECK(cppStandardLibraryRandomValues(n) == uniformIntegerDistribution(mant::Rng::generator_));
        }
        CHECK(arma::all(armadilloRandomValues == arma::randi<arma::uvec>(10)) == true);
      }
    }
  }
}

SCENARIO("Rng::setRandomSeed", "[randomNumberGenerator][Rng::setRandomSeed]") {
  // Based on the implementation details of `std::random_device`, there isn't much we could assert/test about generating random seeds.
  WHEN("The return seed is reset after generating a sequence of random numbers") {
    THEN("Return the same random values again") {
      std::random_device::result_type seed = mant::Rng::setRandomSeed();

      // Tests Armadillo's generator and `mant::Rng::generator_` as both are usually independent.
      arma::uvec::fixed<10> cppStandardLibraryRandomValues;
      std::uniform_int_distribution<arma::uword> uniformIntegerDistribution;
      for (arma::uword n = 0; n < cppStandardLibraryRandomValues.n_elem; ++n) {
        cppStandardLibraryRandomValues(n) = uniformIntegerDistribution(mant::Rng::generator_);
      }
      const arma::uvec::fixed<10> armadilloRandomValues = arma::randi<arma::uvec>(10);

      mant::Rng::setSeed(seed);

      for (arma::uword n = 0; n < cppStandardLibraryRandomValues.n_elem; ++n) {
        CHECK(cppStandardLibraryRandomValues(n) == uniformIntegerDistribution(mant::Rng::generator_));
      }
      CHECK(arma::all(armadilloRandomValues == arma::randi<arma::uvec>(10)) == true);

      arma::mat::fixed<100000, 1> seeds;
      seeds.imbue([]() { return static_cast<double>(mant::Rng::setRandomSeed()); });
      CHECK(isUniformlyDistributed(seeds, std::numeric_limits<std::random_device::result_type>::min(), std::numeric_limits<std::random_device::result_type>::max()) == true);
    }
  }
}
