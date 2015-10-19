#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/roboticsOptimisationProblem.hpp"

namespace mant {
  namespace robotics {
    class EndEffectorPoseError : public RoboticsOptimisationProblem {
     public:
      std::string toString() const override;

     protected:
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
    };
  }
}
