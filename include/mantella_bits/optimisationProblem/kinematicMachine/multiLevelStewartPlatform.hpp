#pragma once

// C++ Standard Library
#include <vector>

// Mantella
#include <mantella_bits/optimisationProblem/kinematicMachine.hpp>
#include <mantella_bits/optimisationProblem/kinematicMachine/parallelKinematicMachine_6pups.hpp>

namespace mant {
  namespace robotic {
    class MultiLevelStewartPlatform : public KinematicMachine {
      public:
        explicit MultiLevelStewartPlatform() noexcept;

        explicit MultiLevelStewartPlatform(
            const std::vector<ParallelKinematicMachine_6PUPS>& platformLevels) noexcept;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        MultiLevelStewartPlatform(const MultiLevelStewartPlatform&) = delete;
        MultiLevelStewartPlatform& operator=(const MultiLevelStewartPlatform&) = delete;

        std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

      protected:
        const std::vector<ParallelKinematicMachine_6PUPS> platformLevels_;
    };
  }
}
