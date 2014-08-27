#include <helper/random.hpp>

namespace hop {
  std::random_device Random::RANDOM_DEVIDE;
  std::mt19937_64 Random::RNG(Random::RANDOM_DEVIDE());
}
