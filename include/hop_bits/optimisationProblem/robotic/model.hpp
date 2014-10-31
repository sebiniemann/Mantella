#pragma once

// C++ STL
#include <vector>

// Armadillo
#include <armadillo>

namespace hop {
  namespace robotic {
    class RobotModel {
      public:

        virtual std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const = 0;

        virtual arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const = 0;

        virtual double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const = 0;
    };
  }
}
