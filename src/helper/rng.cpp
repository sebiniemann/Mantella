#include <hop_bits/helper/rng.hpp>

namespace hop {
  decltype(Rng::generator) Rng::generator;
  decltype(Rng::seed_) Rng::seed_;

  void Rng::setSeed(
      const unsigned int& seed) {
    seed_ = seed;

    generator.seed(seed_);
    arma::arma_rng::set_seed(seed_);

  }

  void Rng::setRandomSeed() {
    arma::arma_rng::set_seed_random();
    setSeed(arma::randi<arma::Col<arma::uword>>(1).at(0));
  }

  unsigned int Rng::getSeed() {
    return seed_;
  }
}
