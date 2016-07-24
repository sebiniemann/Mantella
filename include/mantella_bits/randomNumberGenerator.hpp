#pragma once

// C++ standard library
#include <random>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/config.hpp" // IWYU pragma: keep

namespace mant {
  class Rng {
   public:
    // The random number generator should act as a singleton, no need for default constructors.
    Rng() = delete;
    Rng(const Rng&) = delete;
    Rng(Rng&&) = delete;
    Rng& operator=(const Rng&) = delete;
    Rng& operator=(Rng&&) = delete;

    static std::mt19937_64& getGenerator() {
      if (threadNumber() < generators_.size()) {
        throw std::runtime_error("Rng.getGenerator: The thread number exceeded the maximal number of threads to support.");
      }
      
      return generators_.at(threadNumber());
    }

    static void setSeed(
        const std::random_device::result_type seed) {
      // Uses a different seed for each generator, deterministically based on the provided one.
      for (arma::uword n = 0; n < generators_.size(); ++n) {
        generators_.at(n).seed(seed + n);
      }
    }

    static std::random_device::result_type setRandomSeed();

   protected:
    static std::array<std::mt19937_64, MAXIMAL_NUMBER_OF_THREADS> generators_;
  };
}
