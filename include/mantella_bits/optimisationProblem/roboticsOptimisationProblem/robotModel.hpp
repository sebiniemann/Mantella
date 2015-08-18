#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/printable.hpp>

namespace mant {
  namespace robotics {
    class RobotModel : public Printable {
      public:
        arma::uword numberOfActiveJoints_;
        arma::uword numberOfRedundantJoints_;
        
        explicit RobotModel(
            const arma::uword numberOfActiveJoints,
            const arma::uword numberOfRedundantJoint);

        void setMinimalActiveJointsActuation(
            const arma::Row<double> minimalActiveJointsActuation);
        void setMaximalActiveJointsActuation(
            const arma::Row<double> maximalActiveJointsActuation);

        arma::Row<double> getMinimalActiveJointsActuation() const;
        arma::Row<double> getMaximalActiveJointsActuation() const;

        void setMinimalRedundantJointsActuation(
            const arma::Row<double> minimalRedundantJointsActuation);
        void setMaximalRedundantJointsActuation(
            const arma::Row<double> maximalRedundantJointsActuation);

        arma::Row<double> getMinimalRedundantJointsActuation() const;
        arma::Row<double> getMaximalRedundantJointsActuation() const;
            
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
        arma::Row<double> minimalActiveJointsActuation_;
        arma::Row<double> maximalActiveJointsActuation_;
        
        arma::Row<double> minimalRedundantJointsActuation_;
        arma::Row<double> maximalRedundantJointsActuation_;
      
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