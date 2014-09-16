#pragma once

#include <memory>

#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  std::string to_string(const std::shared_ptr<OptimisationProblem> optimisationProblem);
}
