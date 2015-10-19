#pragma once

// C++ standard library
#include <random>

// Armadillo
#include <armadillo>

namespace mant {
  class Rng {
   public:
    Rng() = delete;
    Rng(const Rng&) = delete;
    Rng& operator=(const Rng&) = delete;

    static std::mt19937_64& getGenerator();

    static void setSeed(
        const arma::arma_rng::seed_type seed);

    static void setRandomSeed();

    static arma::arma_rng::seed_type getSeed();

   protected:
    static arma::arma_rng::seed_type& seed_();
  };
}
