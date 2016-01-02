#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/parallelKinematicMachine6PRUS.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include "mantella_bits/geometry.hpp"

// The following robot configurations and calculations are based on the work of our research colleagues from the Institute of Mechatronic Systems, Leibniz Universität Hannover, Germany.
// To understand the following equations, we advise to carefully read the following paper:
// @see J. Kotlarski et al. (2012). Influence of kinematic redundancy on the singularity-free workspace of parallel kinematic machines. Frontiers of Mechanical Engineering, 7(2), pp. 120-134.
namespace mant {
  namespace krmp {
    ParallelKinematicMachine6PRUS::ParallelKinematicMachine6PRUS()
        : ParallelKinematicMachine6PRUS(
              arma::join_slices(arma::Mat<double>::fixed<3, 6>({-0.050659008749464, 0.360457577021932, -0.6, 0.050659008749464, 0.360457577021932, -0.6, 0.337494923062311, -0.136356800003392, -0.6, 0.286835914312847, -0.224100777018540, -0.6, -0.286835914312847, -0.224100777018540, -0.6, -0.337494923062311, -0.136356800003392, -0.6}), arma::Mat<double>::fixed<3, 6>({-0.050659008749464, 0.360457577021932, 0.2, 0.050659008749464, 0.360457577021932, 0.2, 0.337494923062311, -0.136356800003392, 0.2, 0.286835914312847, -0.224100777018540, 0.2, -0.286835914312847, -0.224100777018540, 0.2, -0.337494923062311, -0.136356800003392, 0.2})),
              {0.927183854566785, 0.374606593415919, 0, 0.927183854566785, -0.374606593415919, 0, -0.139173100960068, -0.990268068741570, 0, -0.788010753606724, -0.615661475325656, 0, -0.788010753606724, 0.615661475325656, 0, -0.139173100960068, 0.990268068741570, 0},
              {0.24, 0.56, 0.24, 0.56, 0.24, 0.56, 0.24, 0.56, 0.24, 0.56, 0.24, 0.56},
              {-0.027346281319362, 0.067684421383375, 0.0, 0.027346281319362, 0.067684421383375, 0.0, 0.072289569018135, -0.010159636370085, 0.0, 0.044943287698773, -0.057524785013291, 0.0, -0.044943287698773, -0.057524785013291, 0.0, -0.072289569018135, -0.010159636370085, 0.0}) {
    }

    // Avoids copying all fields per hand and uses the constructor instead.
    ParallelKinematicMachine6PRUS::ParallelKinematicMachine6PRUS(
        const ParallelKinematicMachine6PRUS& parallelKinematicMachine6PRUS)
        : ParallelKinematicMachine6PRUS(parallelKinematicMachine6PRUS.redundantJoints_, parallelKinematicMachine6PRUS.baseJointNormals_, parallelKinematicMachine6PRUS.linkLengths_, parallelKinematicMachine6PRUS.relativeEndEffectorJoints_) {
    }

    ParallelKinematicMachine6PRUS::ParallelKinematicMachine6PRUS(
        const arma::Cube<double>::fixed<3, 6, 2>& redundantJoints,
        const arma::Mat<double>::fixed<3, 6>& baseJointNormals,
        const arma::Mat<double>::fixed<2, 6>& linkLengths,
        const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints)
        // Linkages with an equal redundant joint start and end position does not have a redundant joint.
        : MachineModel(6, arma::accu(arma::any(redundantJoints.slice(1) - redundantJoints.slice(0)))),
          redundantJoints_(redundantJoints),
          baseJointNormals_(baseJointNormals),
          linkLengths_(linkLengths),
          relativeEndEffectorJoints_(relativeEndEffectorJoints),
          redundantJointDirections_(redundantJoints.slice(1) - redundantJoints.slice(0)),
          redundantJointIndicies_(arma::find(arma::any(redundantJointDirections_))),
          redundantJointAngles_(getRedundantJointAngles(redundantJointDirections_, redundantJointIndicies_)) {
      setModelFunction([this](
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointActuations) {
        assert(redundantJointActuations.n_elem == numberOfRedundantJoints_);
        assert(arma::all(0 <= redundantJointActuations && redundantJointActuations <= 1));

        // The end-effector pose
        const arma::Col<double>::fixed<3>& endEffectorPosition = endEffectorPose.head(3);
        const double endEffectorRollAngle = endEffectorPose(3);
        const double endEffectorPitchAngle = endEffectorPose(4);
        const double endEffectorYawAngle = endEffectorPose(5);
        
        arma::Cube<double>::fixed<3, 6, 3> model;

        // The base joints
        model.slice(0) = redundantJoints_.slice(0);
        // Calculates the actual base joint position, in case the base joint is linked with a redundant joints.
        for (arma::uword n = 0; n < numberOfRedundantJoints_; ++n) {
          const arma::uword redundantJointIndex = redundantJointIndicies_(n);
          model.slice(0).col(redundantJointIndex) += redundantJointActuations(n) * redundantJointDirections_.col(redundantJointIndex);
        }
        
        // The end-effector joints
        // Corrects the orientation of the end-effector ...
        model.slice(2) = rotationMatrix3D(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * relativeEndEffectorJoints_;
        // ... and translate it into the *base* coordinate system.
        model.slice(2).each_col() += endEffectorPosition;
                
        // At this point, the model might be invalid, since the distance between the base and end-effector could make it impossible for both links to intersect.
        // In this case, the middle joint is set to *arma::datum::nan*.
        for (arma::uword n = 0; n < model.slice(0).n_cols; ++n) {
          try {
            // Selects the first intersection point (the other one would also be fine).
            model.slice(1).col(n) = circleSphereIntersections(model.slice(0).col(n), linkLengths_(0, n), baseJointNormals_.col(n), model.slice(2).col(n), linkLengths_(1, n)).col(0);
          } catch (...) {
            model.slice(1).col(n).fill(arma::datum::nan);
          }
        }
        
        return model;
      });

      setPoseInaccuracyFunction([this](
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointActuations) {
        assert(redundantJointActuations.n_elem == numberOfRedundantJoints_);
        assert(arma::all(0 <= redundantJointActuations && redundantJointActuations <= 1));
        
        // The end-effector pose
        const arma::Col<double>::fixed<3>& endEffectorPosition = endEffectorPose.head(3);

        // The robots model
        const arma::Cube<double>::fixed<3, 6, 3>& model = getModel(endEffectorPose, redundantJointActuations);
        const arma::Mat<double>::fixed<3, 6>& baseJoints = model.slice(0);
        const arma::Mat<double>::fixed<3, 6>& middleJoints = model.slice(1);
        const arma::Mat<double>::fixed<3, 6>& endEffectorJoints = model.slice(2);
        
        if (!model.slice(1).is_finite()) {
          // Based on the calculation below (excluding this condition), the worst/maximal objective value would be 1. To give the optimisation algorithm some insight in which direction to go for a valid robot model, we add the deviation from the allowed end-effector distance (including both, the upper and lower bound) to the worst/maximal objective value.
          // *Note:* This extends the procedure described an the aforementioned paper.
          const arma::Row<double> endEffectorDistance = arma::sqrt(arma::sum(arma::square(endEffectorJoints - baseJoints)));
          return 1.0 + arma::accu(arma::max(arma::max(arma::zeros<arma::Row<double>>(numberOfActiveJoints_), arma::abs(linkLengths_.row(1) - linkLengths_.row(0)) - endEffectorDistance), endEffectorDistance - arma::sum(linkLengths_)));
        }
        
        // Some helper variables, makes the following easier to read.
        const arma::Mat<double>::fixed<3, 6>& baseToMiddleJoints = middleJoints - baseJoints;
        const arma::Mat<double>::fixed<3, 6>& middleToEndEffectorJoints = endEffectorJoints - middleJoints;
        const arma::Mat<double>::fixed<3, 6>& baseToEndEffectorJoints = endEffectorJoints - baseJoints;
        // Reverses the translation into the *base* coordinate system.
        const arma::Mat<double>::fixed<3, 6>& rotatedRelativeEndEffectorJoints = endEffectorJoints.each_col() - endEffectorPosition;
        
        // The forward kinematic 
        // *Note:* We fill the transpose of the forward kinematic, as this fits better with the shape of other matrices. This matrix is then later on transposed to get the actually intended one.
        arma::Mat<double>::fixed<6, 6> forwardKinematic;
        forwardKinematic.head_rows(3) = middleToEndEffectorJoints;
        for (arma::uword n = 0; n < forwardKinematic.n_rows; ++n) {
          forwardKinematic.submat(3, n, 5, n) = arma::cross(rotatedRelativeEndEffectorJoints.col(n), middleToEndEffectorJoints.col(n));
        }
        
        // The inverse kinematic
        arma::Mat<double> inverseKinematic(6, 6 + redundantJointIndicies_.n_elem, arma::fill::zeros);
        inverseKinematic.diag() = baseToEndEffectorJoints.row(0) % baseToMiddleJoints.row(1) - baseToEndEffectorJoints.row(1) % baseToMiddleJoints.row(0);

        for (arma::uword n = 0; n < numberOfRedundantJoints_; ++n) {
          const arma::uword redundantJointIndex = redundantJointIndicies_(n);
          inverseKinematic(redundantJointIndex, 6 + n) = arma::dot(middleToEndEffectorJoints.col(redundantJointIndex), redundantJointAngles_.col(n));
        }
        
        // Shifts the actual objective value (the negative inverse of the condition number) by 1, to let 0 be the best/lowest possible objective value.
        return 1.0 - 1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
      });
    }

    arma::Mat<double> ParallelKinematicMachine6PRUS::getRedundantJointAngles(
        const arma::Mat<double>::fixed<3, 6> redundantJointDirections,
        const arma::Col<arma::uword> redundantJointIndicies) const {
      arma::Mat<double> redundantJointAngles(3, redundantJointIndicies_.n_elem);

      for (arma::uword n = 0; n < redundantJointIndicies.n_elem; ++n) {
        const arma::uword redundantJointIndex = redundantJointIndicies(n);
        const double redundantJointXAngle = std::atan2(redundantJointDirections(1, redundantJointIndex), redundantJointDirections(0, redundantJointIndex));
        const double redundantJointYAngle = std::atan2(redundantJointDirections(2, redundantJointIndex), redundantJointDirections(1, redundantJointIndex));
        redundantJointAngles.col(n) = arma::Col<double>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }

      return redundantJointAngles;
    }
  }
}
