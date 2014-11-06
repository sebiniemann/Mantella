#pragma once

// C++ Standard Library
#include <vector>

// HOP
#include <hop_bits/optimisationProblem/robotic/model.hpp>
#include <hop_bits/optimisationProblem/robotic/model/parallelKinematicMachine_6pups.hpp>

namespace hop {
  namespace robotic {
    class MultiLevelStewartPlatform : public Model {
      public:
        MultiLevelStewartPlatform(
            const std::vector<ParallelKinematicMachine_6PUPS>& platformLevels);

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        MultiLevelStewartPlatform(const MultiLevelStewartPlatform&) = delete;
        MultiLevelStewartPlatform& operator=(const MultiLevelStewartPlatform&) = delete;

        std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const override;

        arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const override;

        double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const override;

      protected:
        const std::vector<ParallelKinematicMachine_6PUPS> platformLevels_;
    };
  }
}
