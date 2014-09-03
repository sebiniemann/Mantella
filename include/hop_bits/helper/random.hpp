#pragma once

#include <random>

namespace hop {
  class Random {
    public:
      static std::mt19937_64 RNG;

    protected:
      static std::random_device RANDOM_DEVIDE;
  };
}
