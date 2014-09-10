#include <hop_bits/helper/random.hpp>

namespace hop {
  decltype(Random::RandomDevice) Random::RandomDevice;
  decltype(Random::Rng) Random::Rng(Random::RandomDevice());
}
