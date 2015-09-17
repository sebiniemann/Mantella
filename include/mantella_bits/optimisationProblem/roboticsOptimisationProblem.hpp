#pragma once

// C++ standard library
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationProblem.hpp>
#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp>

namespace mant {
  namespace robotics {
    class RoboticsOptimisationProblem : public OptimisationProblem {
     public:
      RoboticsOptimisationProblem(
          std::shared_ptr<RobotModel> robotModel);

      void setEndEffectorTrajectory(
          const arma::Mat<double>& endEffectorTrajectory);

      arma::Mat<double> getEndEffectorTrajectory() const;

      virtual ~RoboticsOptimisationProblem() = default;

     protected:
      std::shared_ptr<RobotModel> robotModel_;

      arma::Mat<double> endEffectorTrajectory_;
    };
  }
}
