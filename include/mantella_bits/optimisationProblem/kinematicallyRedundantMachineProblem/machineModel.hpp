#pragma once

// C++ standard library
#include <functional>

// Armadillo
#include <armadillo>

namespace mant {
  namespace krmp {
    class MachineModel {
     public:
      const arma::uword numberOfActiveJoints_;
      const arma::uword numberOfRedundantJoints_;

      explicit MachineModel(
          const arma::uword numberOfActiveJoints,
          const arma::uword numberOfRedundantJoint);

      void setModelFunction(
          const std::function<arma::Cube<double>(const arma::Col<double>& endEffectorPose, const arma::Row<double>& redundantJointsActuation)> modelFunction);
      arma::Cube<double> getModel(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const;

      void setPoseInaccuracyFunction(
          const std::function<double(const arma::Col<double>& endEffectorPose, const arma::Row<double>& redundantJointsActuation)> poseInaccuracyFunction);
      double getPoseInaccuracy(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const;

     protected:
      std::function<arma::Cube<double>(const arma::Col<double>& endEffectorPose, const arma::Row<double>& redundantJointsActuation)> modelFunction_;
      std::function<double(const arma::Col<double>& endEffectorPose, const arma::Row<double>& redundantJointsActuation)> poseInaccuracyFunction_;
    };
  }
}
