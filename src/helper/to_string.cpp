#include <hop_bits/helper/to_string.hpp>

namespace hop {
  std::string to_string(const std::shared_ptr<OptimisationProblem> optimisationProblem) {
    return optimisationProblem->to_string();
  }
}
