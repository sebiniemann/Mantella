// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("Rng::setSeed", "[randomNumberGenerator][Rng::setSeed]") {
  GIVEN("A seed") {
    WHEN("The seed is reset after generating a sequence of random numbers") {
      THEN("Return the same random values again") {
        mant::Rng::setSeed(12345);
        arma::vec randomValues = mant::uniformRandomNumbers(10);

        mant::Rng::setSeed(12345);
        CHECK(arma::approx_equal(randomValues, mant::uniformRandomNumbers(10), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }
}

SCENARIO("Rng::setRandomSeed", "[randomNumberGenerator][Rng::setRandomSeed]") {
  // Based on the implementation details of `std::random_device`, there isn't
  // much we could assert/test about generating random seeds.
  WHEN("The return seed is reset after generating a sequence of random numbers") {
    THEN("Return the same random values again") {
      arma::vec::fixed<10000> seeds;
      seeds.imbue([]() { return static_cast<double>(mant::Rng::setRandomSeed()); });
      CHECK(isUniformDistributed(seeds, std::numeric_limits<std::random_device::result_type>::min(), std::numeric_limits<std::random_device::result_type>::max()) == true);
    }
  }
}
