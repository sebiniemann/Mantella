#include <hop_bits/helper/random.hpp>

namespace hop {
  decltype(Random::RANDOM_DEVIDE) Random::RANDOM_DEVIDE;
  decltype(Random::RNG) Random::RNG(Random::RANDOM_DEVIDE());
}
