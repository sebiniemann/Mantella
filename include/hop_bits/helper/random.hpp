#pragma once

#include <random>

#include <armadillo>

namespace hop {
  class Random {
    public:
      Random() = delete;
      Random(const Random&) = delete;
      Random& operator=(const Random&) = delete;

      static std::mt19937_64 Rng;

      static void setSeed(const unsigned int& seed);
      static void setRandomSeed();

      static arma::Mat<double> getRandomRotationMatrix(unsigned int numberOfDimensions);

      static arma::Col<arma::uword> getRandomPermutation(unsigned int numberOfElements, unsigned int cycleLength);
      static arma::Col<arma::uword> getRandomPermutation(unsigned int numberOfElements);

    protected:
      static std::random_device RandomDevice;
  };
}
