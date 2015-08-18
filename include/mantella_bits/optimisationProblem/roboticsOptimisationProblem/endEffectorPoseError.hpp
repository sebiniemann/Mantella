#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem.hpp>

namespace mant {
  namespace robotics {
    class EndEffectorPoseError : public RoboticsOptimisationProblem {
      public:
        std::string toString() const;
        
      protected:
      
        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const override;
    };
  }
}
