#pragma once

// C++ standard library
#include <random>

// Armadillo
#include <armadillo>

namespace mant {
  class Rng {
   public:
    // The random number generator should act as a singleton, no need for default constructors.
    Rng() = delete;
    Rng(const Rng&) = delete;
    Rng& operator=(const Rng&) = delete;

    // The std::mersenne_twister_engine requires an l-value (a reference in this case) instead of an r-value.
    // Otherwise, calls like std::uniform_real_distribution<double>(0, 1)(Rng::getGenerator()) won't compile.
    static std::mt19937_64& getGenerator();

    static void setSeed(
        const arma::arma_rng::seed_type seed);

    static void setRandomSeed();

    static arma::arma_rng::seed_type getSeed();

   protected:
    static arma::arma_rng::seed_type seed_;
    static std::mt19937_64 generator_;
  };
}
