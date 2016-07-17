#pragma once

// C++ standard library
#include <random>

namespace mant {
  class Rng {
   public:
    // The random number generator should act as a singleton, no need for default constructors.
    Rng() = delete;
    Rng(const Rng&) = delete;
    Rng(Rng&&) = delete;
    Rng& operator=(const Rng&) = delete;
    Rng& operator=(Rng&&) = delete;

    static std::mt19937_64 generator_;

    static void setSeed(
        const std::random_device::result_type seed);

    static std::random_device::result_type setRandomSeed();
  };
}
