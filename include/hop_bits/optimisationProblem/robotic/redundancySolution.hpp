#pragma once

#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class RedundancySolution : public OptimisationProblem {
    protected:
      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const;
  };
}
