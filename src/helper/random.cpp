#include <helper/random.hpp>

namespace hop {
  random_device Random::RANDOM_DEVIDE;
  mt19937_64 Random::RNG(Random::RANDOM_DEVIDE());
}
