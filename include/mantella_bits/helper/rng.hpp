#pragma once

// C++ standard library
#include <random>

namespace mant {
  class Rng {
    public:
      Rng() = delete;
      Rng(const Rng&) = delete;
      Rng& operator=(const Rng&) = delete;

      static std::mt19937_64& getGenerator();

      static void setSeed(
          const std::mt19937_64::result_type seed);

      static void setRandomSeed();

      static std::mt19937_64::result_type getSeed();

    protected:
      static std::mt19937_64::result_type& seed_();
  };
}
