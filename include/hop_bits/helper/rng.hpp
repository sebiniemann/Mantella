#pragma once

// C++ Standard Library
#include <random>

// Armadillo
#include <armadillo>

namespace hop {
  class Rng {
    public:
      Rng() = delete;
      Rng(const Rng&) = delete;
      Rng& operator=(const Rng&) = delete;

      static std::mt19937_64 generator;

      static void setSeed(
          const unsigned int& seed);

      static void setRandomSeed();

      static unsigned int getSeed();

    protected:
      static unsigned int seed_;
  };
}
