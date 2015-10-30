#include "mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine3PRPR.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include "mantella_bits/helper/geometry.hpp"

namespace mant {
  namespace robotics {
    ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR()
        : ParallelKinematicMachine3PRPR(
              // endEffectorJointPositions
              {-0.000066580445834, 0.106954081945581, -0.092751709777083, -0.053477040972790, 0.092818290222917, -0.053477040972790},
              // minimalActiveJointsActuation
              {0.1, 0.1, 0.1},
              // maximalActiveJointsActuation
              {1.2, 1.2, 1.2},
              // redundantJointStartPositions
              {0.1, 1.0392, 0.0, 0.8, 1.2, 0.8},
              // redundantJointEndPositions
              {1.1, 1.0392, 0.0, -0.2, 1.2, -0.2}) {
    }

    ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR(
        const ParallelKinematicMachine3PRPR& parallelKinematicMachine3PRPR)
        : ParallelKinematicMachine3PRPR(parallelKinematicMachine3PRPR.endEffectorJointPositions_, parallelKinematicMachine3PRPR.minimalActiveJointsActuation_, parallelKinematicMachine3PRPR.maximalActiveJointsActuation_, parallelKinematicMachine3PRPR.redundantJointStartPositions_, parallelKinematicMachine3PRPR.redundantJointEndPositions_) {
    }

    ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR(
        const arma::Mat<double>::fixed<2, 3>& endEffectorJointPositions,
        const arma::Row<double>::fixed<3>& minimalActiveJointsActuation,
        const arma::Row<double>::fixed<3>& maximalActiveJointsActuation,
        const arma::Mat<double>::fixed<2, 3>& redundantJointStartPositions,
        const arma::Mat<double>::fixed<2, 3>& redundantJointEndPositions)
        : RobotModel(3, static_cast<arma::Col<double>>(arma::nonzeros(redundantJointEndPositions - redundantJointStartPositions)).n_elem),
          endEffectorJointPositions_(endEffectorJointPositions),
          minimalActiveJointsActuation_(minimalActiveJointsActuation),
          maximalActiveJointsActuation_(maximalActiveJointsActuation),
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

    arma::Cube<double> ParallelKinematicMachine3PRPR::getModelImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(arma::all(redundantJointsActuation >= 0) && arma::all(redundantJointsActuation <= 1));

      arma::Cube<double>::fixed<2, 3, 2> model;

      const arma::Col<double>::fixed<2>& endEffectorPosition = endEffectorPose.subvec(0, 1);
      const double endEffectorAngle = endEffectorPose(2);

      model.slice(0) = redundantJointStartPositions_;
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const arma::uword redundantJointIndex = redundantJointIndicies_(n);
        model.slice(0).col(redundantJointIndex) += redundantJointsActuation(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      model.slice(1) = get2DRotation(endEffectorAngle) * endEffectorJointPositions_;
      model.slice(1).each_col() += endEffectorPosition;

      return model;
    }

    arma::Row<double> ParallelKinematicMachine3PRPR::getActuationImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(arma::all(redundantJointsActuation >= 0) && arma::all(redundantJointsActuation <= 1));

      const arma::Cube<double>::fixed<2, 3, 2>& model = getModel(endEffectorPose, redundantJointsActuation);

      const arma::Mat<double>::fixed<2, 3>& baseJoints = model.slice(0);
      const arma::Mat<double>::fixed<2, 3>& endEffectorJoints = model.slice(1);

      return arma::sqrt(arma::sum(arma::square(endEffectorJoints - baseJoints)));
    }

    double ParallelKinematicMachine3PRPR::getEndEffectorPoseErrorImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(arma::all(redundantJointsActuation >= 0) && arma::all(redundantJointsActuation <= 1));

      const arma::Cube<double>::fixed<2, 3, 2>& model = getModel(endEffectorPose, redundantJointsActuation);

      const arma::Mat<double>::fixed<2, 3>& baseJointPositions = model.slice(0);

      const arma::Mat<double>::fixed<2, 3>& endEffectorJointPositions = model.slice(1);
      arma::Mat<double>::fixed<2, 3> endEffectorJointPositionsRotated = endEffectorJointPositions;
      endEffectorJointPositionsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      const arma::Mat<double>::fixed<2, 3>& baseToEndEffectorJointPositions = endEffectorJointPositions - baseJointPositions;
      const arma::Row<double>::fixed<3>& baseToEndEffectorJointsActuation = arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));

      if (arma::any(baseToEndEffectorJointsActuation < minimalActiveJointsActuation_) || arma::any(baseToEndEffectorJointsActuation > maximalActiveJointsActuation_)) {
        return 0.0;
      }

      arma::Mat<double>::fixed<3, 3> forwardKinematic;
      forwardKinematic.head_rows(2) = baseToEndEffectorJointPositions;
      forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointPositionsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointPositionsRotated.row(0);

      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = -baseToEndEffectorJointsActuation;
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const arma::uword redundantJointIndex = redundantJointIndicies_(n);
        inverseKinematic(n, 3 + n) = forwardKinematic(redundantJointIndex, 0) * redundantJointAngleCosines_(n) + forwardKinematic(redundantJointIndex, 1) * redundantJointAngleSines_(n);
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }

    std::string ParallelKinematicMachine3PRPR::toString() const {
      return "robotics_parallel_kinematic_machine_3prpr";
    }
  }
}
