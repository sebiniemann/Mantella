#pragma once

#include <random>
using std::random_device;
using std::mt19937_64;

namespace hop {
  class Random {
    public:
      static mt19937_64 RNG;

    protected:
      static random_device RANDOM_DEVIDE;
  };
}
