#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/printable.hpp>

namespace mant {
  namespace robotics {
    class RobotModel : public Printable {
     public:
      const arma::uword numberOfActiveJoints_;
      const arma::uword numberOfRedundantJoints_;

      explicit RobotModel(
          const arma::uword numberOfActiveJoints,
          const arma::uword numberOfRedundantJoint);

      arma::Cube<double> getModel(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const;

      arma::Row<double> getActuation(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const;

      double getEndEffectorPoseError(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const;

      virtual ~RobotModel() = default;

     protected:
      virtual arma::Cube<double> getModelImplementation(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const = 0;

      virtual arma::Row<double> getActuationImplementation(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const = 0;

      virtual double getEndEffectorPoseErrorImplementation(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const = 0;
    };
  }
}
