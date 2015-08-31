#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine3PRRR.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/geometry.hpp>

namespace mant {
  namespace robotics {
    ParallelKinematicMachine3PRRR::ParallelKinematicMachine3PRRR() 
      : ParallelKinematicMachine3PRRR(
          // linkLengths
          {0.6, 0.6,
           0.6, 0.6,
           0.6, 0.6},
          // endEffectorJointPositions
          {-0.000066580445834,  0.106954081945581,
           -0.092751709777083, -0.053477040972790,
            0.092818290222917, -0.053477040972790},
          // redundantJointStartPositions
          {0.1, 1.0392,
           0.0, 0.8,
           1.2, 0.8},
          // redundantJointEndPositions
          {1.1, 1.0392,
           0.0, -0.2,
           1.2, -0.2}) {

    }
         
    ParallelKinematicMachine3PRRR::ParallelKinematicMachine3PRRR(
        const ParallelKinematicMachine3PRRR& parallelKinematicMachine3PRRR) 
      : ParallelKinematicMachine3PRRR(parallelKinematicMachine3PRRR.linkLengths_, parallelKinematicMachine3PRRR.endEffectorJointPositions_, parallelKinematicMachine3PRRR.redundantJointStartPositions_, parallelKinematicMachine3PRRR.redundantJointEndPositions_) {
        
    }

    ParallelKinematicMachine3PRRR::ParallelKinematicMachine3PRRR(
        const arma::Mat<double>::fixed<2, 3>& linkLengths,
        const arma::Mat<double>::fixed<2, 3>& endEffectorJointPositions,
        const arma::Mat<double>::fixed<2, 3>& redundantJointStartPositions,
        const arma::Mat<double>::fixed<2, 3>& redundantJointEndPositions)
      : RobotModel(3, static_cast<arma::Col<double>>(arma::nonzeros(redundantJointEndPositions - redundantJointStartPositions)).n_elem),
        linkLengths_(linkLengths),
        endEffectorJointPositions_(endEffectorJointPositions),
        redundantJointStartPositions_(redundantJointStartPositions),
        redundantJointEndPositions_(redundantJointEndPositions),
        redundantJointStartToEndPositions_(redundantJointEndPositions_ - redundantJointStartPositions_),
        redundantJointIndicies_(arma::find(arma::any(redundantJointStartToEndPositions_))),
        redundantJointAngleSines_(redundantJointIndicies_.n_elem),
        redundantJointAngleCosines_(redundantJointIndicies_.n_elem) {
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double redundantJointAngle = std::atan2(redundantJointStartToEndPositions_(1, n), redundantJointStartToEndPositions_(0, n));
        redundantJointAngleSines_(n) = std::sin(redundantJointAngle);
        redundantJointAngleCosines_(n) = std::cos(redundantJointAngle);
      }
    }

    arma::Cube<double> ParallelKinematicMachine3PRRR::getModelImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1));

      arma::Cube<double>::fixed<2, 3, 3> model;

      const arma::Col<double>::fixed<2>& endEffectorPosition = endEffectorPose.subvec(0, 1);
      const double endEffectorAngle = endEffectorPose(2);

      model.slice(0) = redundantJointStartPositions_;
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const arma::uword redundantJointIndex = redundantJointIndicies_(n);
        model.slice(0).col(redundantJointIndex) += redundantJointsActuation(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      model.slice(2) = get2DRotation(endEffectorAngle) * endEffectorJointPositions_;
      model.slice(2).each_col() += endEffectorPosition;

      for (arma::uword n = 0; n < model.slice(0).n_cols; ++n) {
        model.slice(1).col(n) = getCircleCircleIntersection(model.slice(0).col(n), linkLengths_(0, n), model.slice(2).col(n), linkLengths_(1, n));
      }

      return model;
    }

    arma::Row<double> ParallelKinematicMachine3PRRR::getActuationImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1));
      
      const arma::Cube<double>::fixed<2, 3, 3>& model = getModel(endEffectorPose, redundantJointsActuation);

      const arma::Mat<double>::fixed<2, 3>& baseJointPositions = model.slice(0);
      const arma::Mat<double>::fixed<2, 3>& passiveJointPositions = model.slice(1);

      const arma::Mat<double>::fixed<2, 3>& baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<3> actuation;
      for (arma::uword n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation(n) = std::atan2(baseToPassiveJointPositions(1, n), baseToPassiveJointPositions(0, n));
      }

      return actuation;
    }

    double ParallelKinematicMachine3PRRR::getEndEffectorPoseErrorImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1));
      
      const arma::Cube<double>::fixed<2, 3, 3>& model = getModel(endEffectorPose, redundantJointsActuation);

      const arma::Mat<double>::fixed<2, 3>& baseJoints = model.slice(0);

      const arma::Mat<double>::fixed<2, 3>& endEffectorJoints = model.slice(2);
      arma::Mat<double>::fixed<2, 3> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      const arma::Mat<double>::fixed<2, 3>& passiveJoints = model.slice(1);

      arma::Mat<double>::fixed<3, 3> forwardKinematic;
      forwardKinematic.head_rows(2) = endEffectorJoints - passiveJoints;
      forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointsRotated.row(0);

      const arma::Mat<double>::fixed<2, 3>& baseToPassiveJoints = passiveJoints - baseJoints;
      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % baseToPassiveJoints.row(1) - forwardKinematic.row(1) % baseToPassiveJoints.row(0);
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const arma::uword redundantJointIndex = redundantJointIndicies_(n);
        inverseKinematic(n, 3 + n) = -(forwardKinematic(redundantJointIndex, 0) * redundantJointAngleCosines_(n) + forwardKinematic(redundantJointIndex, 1) * redundantJointAngleSines_(n));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
    
    std::string ParallelKinematicMachine3PRRR::toString() const {
      return "robotics_parallel_kinematic_machine_3prrr";
    }
  }
}
