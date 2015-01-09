#include <mantella_bits/helper/rng.hpp>

namespace mant {
  decltype(Rng::generator) Rng::generator;
  decltype(Rng::seed_) Rng::seed_;

  void Rng::setSeed(
      const unsigned int& seed) noexcept {
    seed_ = seed;

    generator.seed(seed_);
    arma::arma_rng::set_seed(seed_);

  }

  void Rng::setRandomSeed() noexcept {
    arma::arma_rng::set_seed_random();
    setSeed(arma::randi<arma::Col<unsigned int>>(1).at(0));
  }

  unsigned int Rng::getSeed() noexcept {
    return seed_;
  }
}
