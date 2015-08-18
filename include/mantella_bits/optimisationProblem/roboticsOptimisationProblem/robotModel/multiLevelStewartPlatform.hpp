#pragma once

// C++ standard library
#include <vector>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp>
#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine6PUPS.hpp>

namespace mant {
  namespace robotics {
    class MultiLevelStewartPlatform : public RobotModel {
      public:
        const arma::uword numberOfPlatformLevels_;
      
        explicit MultiLevelStewartPlatform(
            const arma::uword numberOfPlatformLevels,
            const arma::uword numberOfRedundantJoints);

        void setPlatformLevels(
            const std::vector<ParallelKinematicMachine6PUPS>& platformLevels);

        std::vector<ParallelKinematicMachine6PUPS> getPlatformLevels() const;
            
        std::string toString() const;

      protected:
        std::vector<ParallelKinematicMachine6PUPS> platformLevels_;
        
        arma::Cube<double> getModelImplementation(
            const arma::Col<double>& endEffectorPose,
            const arma::Row<double>& redundantJointsActuation) const override;

        arma::Row<double> getActuationImplementation(
            const arma::Col<double>& endEffectorPose,
            const arma::Row<double>& redundantJointsActuation) const override;

        double getEndEffectorPoseErrorImplementation(
            const arma::Col<double>& endEffectorPose,
            const arma::Row<double>& redundantJointsActuation) const override;
    };
  }
}
