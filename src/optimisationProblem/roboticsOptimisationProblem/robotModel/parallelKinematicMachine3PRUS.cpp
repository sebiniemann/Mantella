#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine3PRUS.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/geometry.hpp>

namespace mant {
  namespace robotics {
    ParallelKinematicMachine3PRUS::ParallelKinematicMachine3PRUS()
      : ParallelKinematicMachine3PRUS(
          // linkLengths
          {0.24, 0.56,
           0.24, 0.56,
           0.24, 0.56},
          // endEffectorJointPositions
          {-0.027346281319362, 0.067684421383375, 0.0,
            0.072289569018135, -0.010159636370085, 0.0,
           -0.044943287698773, -0.057524785013291, 0.0},
          // baseJointRotationAngles
          {0.0, 6.143558967020040,
           0.0, 1.954768762233649,
           0.0, 4.049163864626845},
          // redundantJointStartPositions
          {-0.050659008749464,  0.360457577021932, -0.6,
            0.337494923062311, -0.136356800003392, -0.6,
           -0.286835914312847, -0.224100777018540, -0.6},
          // redundantJointEndPositions
          {-0.050659008749464,  0.360457577021932, 0.2,
            0.337494923062311, -0.136356800003392, 0.2,
           -0.286835914312847, -0.224100777018540, 0.2}) {

    }
         
    ParallelKinematicMachine3PRUS::ParallelKinematicMachine3PRUS(
        const ParallelKinematicMachine3PRUS& parallelKinematicMachine3PRUS) 
      : ParallelKinematicMachine3PRUS(parallelKinematicMachine3PRUS.linkLengths_, parallelKinematicMachine3PRUS.endEffectorJointPositions_, parallelKinematicMachine3PRUS.baseJointRotationAngles_, parallelKinematicMachine3PRUS.redundantJointStartPositions_, parallelKinematicMachine3PRUS.redundantJointEndPositions_) {
        
    }
            
    ParallelKinematicMachine3PRUS::ParallelKinematicMachine3PRUS(
        const arma::Mat<double>::fixed<2, 3>& linkLengths,
        const arma::Mat<double>::fixed<3, 3>& endEffectorJointPositions,
        const arma::Mat<double>::fixed<2, 3>& baseJointRotationAngles,
        const arma::Mat<double>::fixed<3, 3>& redundantJointStartPositions,
        const arma::Mat<double>::fixed<3, 3>& redundantJointEndPositions)
      : RobotModel(3, static_cast<arma::Col<double>>(arma::nonzeros(redundantJointEndPositions - redundantJointStartPositions)).n_elem),
        linkLengths_(linkLengths),
        endEffectorJointPositions_(endEffectorJointPositions),
        baseJointRotationAngles_(baseJointRotationAngles),
        redundantJointStartPositions_(redundantJointStartPositions),
        redundantJointEndPositions_(redundantJointEndPositions),
        redundantJointStartToEndPositions_(redundantJointEndPositions_ - redundantJointStartPositions_),
        redundantJointIndicies_(arma::find(arma::any(redundantJointStartToEndPositions_))),
        redundantJointRotationAngles_(3, redundantJointIndicies_.n_elem) {
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double redundantJointXAngle = std::atan2(redundantJointStartToEndPositions_(1, n), redundantJointStartToEndPositions_(0, n));
        const double redundantJointYAngle = std::atan2(redundantJointStartToEndPositions_(2, n), redundantJointStartToEndPositions_(1, n));
        redundantJointRotationAngles_.col(n) = arma::Col<double>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }

      for (arma::uword n = 0; n < baseJointRotationAngles_.n_cols; ++n) {
        baseJointRotations_.slice(n) = get3DRotation(baseJointRotationAngles_(0, n), 0, baseJointRotationAngles_(1, n));
      }

      for (arma::uword n = 0; n < baseJointRotations_.n_slices; ++n) {
        baseJointNormals_.col(n) = arma::normalise(arma::cross(baseJointRotations_.slice(n).col(0), baseJointRotations_.slice(n).col(2)));
      }
    }

    arma::Cube<double> ParallelKinematicMachine3PRUS::getModelImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1));

      arma::Cube<double>::fixed<3, 3, 3> model;

      const arma::Col<double>::fixed<3>& endEffectorPosition = endEffectorPose.subvec(0, 2);
      const double endEffectorRollAngle = endEffectorPose(3);
      const double endEffectorPitchAngle = endEffectorPose(4);
      const double endEffectorYawAngle = endEffectorPose(5);

      model.slice(0) = redundantJointStartPositions_;
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const arma::uword redundantJointIndex = redundantJointIndicies_(n);
        model.slice(0).col(redundantJointIndex) += redundantJointsActuation(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      model.slice(2) = get3DRotation(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointPositions_;
      model.slice(2).each_col() += endEffectorPosition;

      for (arma::uword n = 0; n < model.slice(0).n_cols; ++n) {
        model.slice(1).col(n) = getCircleSphereIntersection(model.slice(0).col(n), linkLengths_(0, n), baseJointNormals_.col(n), model.slice(2).col(n), linkLengths_(1, n));
      }

      return model;
    }

    arma::Row<double> ParallelKinematicMachine3PRUS::getActuationImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1));
      
      const arma::Cube<double>::fixed<3, 3, 3>& model = getModel(endEffectorPose, redundantJointsActuation);

      const arma::Mat<double>::fixed<3, 3>& baseJointPositions = model.slice(0);
      const arma::Mat<double>::fixed<3, 3>& passiveJointPositions = model.slice(1);

      const arma::Mat<double>::fixed<3, 3>& baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<3> actuation;
      for (arma::uword n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation(n) = std::atan2(baseToPassiveJointPositions(0, n), baseToPassiveJointPositions(2, n));
      }

      return actuation;
    }

    double ParallelKinematicMachine3PRUS::getEndEffectorPoseErrorImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1));
      
      const arma::Cube<double>::fixed<3, 3, 3>& model = getModel(endEffectorPose, redundantJointsActuation);

      const arma::Mat<double>::fixed<3, 3>& baseJoints = model.slice(0);

      const arma::Mat<double>::fixed<3, 3>& endEffectorJoints = model.slice(2);
      arma::Mat<double>::fixed<3, 3> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 2);

      const arma::Mat<double>::fixed<3, 3>& passiveJoints = model.slice(1);

      arma::Mat<double>::fixed<3, 3> relativeBaseToPassiveJoints = passiveJoints - baseJoints;
      for (arma::uword n = 0; n < relativeBaseToPassiveJoints.n_cols; ++n) {
        relativeBaseToPassiveJoints.col(n) = baseJointRotations_.slice(n) * relativeBaseToPassiveJoints.col(n);
      }

      const arma::Mat<double>::fixed<3, 3>& baseToEndEffectorJoints = endEffectorJoints - baseJoints;

      arma::Mat<double>::fixed<3, 3> forwardKinematic;
      forwardKinematic.head_rows(3) = baseToEndEffectorJoints;
      for (arma::uword n = 0; n < baseToEndEffectorJoints.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJoints.col(n));
      }

      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % relativeBaseToPassiveJoints.row(1) - forwardKinematic.row(1) % relativeBaseToPassiveJoints.row(0);
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const arma::uword redundantJointIndex = redundantJointIndicies_(n);
        inverseKinematic(n, 3 + n) = arma::dot(baseToEndEffectorJoints.col(redundantJointIndex), redundantJointRotationAngles_.col(redundantJointIndex));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
    
    std::string ParallelKinematicMachine3PRUS::toString() const {
      return "robotics_parallel_kinematic_machine_3prus";
    }
  }
}
