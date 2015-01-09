#pragma once

// C++ Standard Library
#include <random>

// Armadillo
#include <armadillo>

namespace mant {
  class Rng {
    public:
      Rng() = delete;
      Rng(const Rng&) = delete;
      Rng& operator=(const Rng&) = delete;

      static std::mt19937_64 generator;

      static void setSeed(
          const unsigned int& seed) noexcept;

      static void setRandomSeed() noexcept;

      static unsigned int getSeed() noexcept;

    protected:
      static unsigned int seed_;
  };
}
