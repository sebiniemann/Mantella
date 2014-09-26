#pragma once

#include <random>

namespace hop {
  class Random {
    public:
      Random() = delete;
      Random(const Random&) = delete;
      Random& operator=(const Random&) = delete;

      static std::mt19937_64 Rng;

    protected:
      static std::random_device RandomDevice;
  };
}
