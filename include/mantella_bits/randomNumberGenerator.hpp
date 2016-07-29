#pragma once

// C++ standard library
#include <array>
#include <random>
#include <stdexcept>

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

    // This must be implemented within the header as we want to be able to readjust `MAXIMAL_NUMBER_OF_THREADS` simply by predefining it prior to including Mantella the first time, without the need to recompile the library.
    // Therefore, everything that depends on `MAXIMAL_NUMBER_OF_THREADS` in any way needs to be header-only.
    static std::mt19937_64& getGenerator() {
      // **Note:** C++ guarantees under 7.1.2/4 that even if multiple threads attempt to initialize the same static local variable concurrently, the initialisation occurs exactly once.
      static std::array<std::mt19937_64, MAXIMAL_NUMBER_OF_THREADS> generators_;

      if (threadNumber() >= generators_.size()) {
        throw std::runtime_error("Rng.getGenerator: The thread number exceeded the maximal number of threads to support.");
      }

      return generators_.at(threadNumber());
    }

    // This must be implemented within the header as we want to be able to readjust `MAXIMAL_NUMBER_OF_THREADS` simply by predefining it prior to including Mantella the first time, without the need to recompile the library.
    // Therefore, everything that depends on `MAXIMAL_NUMBER_OF_THREADS` in any way needs to be header-only.
    static void setSeed(
        const std::random_device::result_type seed) {
// Since we use a local static variable to hold all generators - which is only accessible by calling `.getGenerator()` for each thread - we use OpenMP to iterate over `.getGenerator()` once per thread.
#pragma omp parallel num_threads(MAXIMAL_NUMBER_OF_THREADS)
      {
#pragma omp for schedule(static)
        for (arma::uword n = 0; n < MAXIMAL_NUMBER_OF_THREADS; ++n) {
          // Uses a different seed for each generator, deterministically based on the provided one.
          getGenerator().seed(seed + n);
        }
      }
    }

    static std::random_device::result_type setRandomSeed();
  };
}
