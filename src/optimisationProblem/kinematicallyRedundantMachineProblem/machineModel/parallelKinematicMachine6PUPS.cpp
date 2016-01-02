#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/parallelKinematicMachine6PUPS.hpp"

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
    ParallelKinematicMachine6PUPS::ParallelKinematicMachine6PUPS()
        : ParallelKinematicMachine6PUPS(
              arma::join_slices(arma::Mat<double>::fixed<3, 6>({-0.463708870031622, 0.417029254828353, -0.346410161513775, 0.463708870031622, 0.417029254828353, -0.346410161513775, 0.593012363818459, 0.193069033993384, -0.346410161513775, 0.129303493786838, -0.610098288821738, -0.346410161513775, -0.129303493786837, -0.610098288821738, -0.346410161513775, -0.593012363818459, 0.193069033993384, -0.346410161513775}), arma::Mat<double>::fixed<3, 6>({-0.247202519085512, 0.292029254828353, 0.086602540378444, 0.247202519085512, 0.292029254828353, 0.086602540378444, 0.376506012872349, 0.068069033993384, 0.086602540378444, 0.129303493786838, -0.360098288821738, 0.086602540378444, -0.129303493786837, -0.360098288821738, 0.086602540378444, -0.376506012872349, 0.068069033993384, 0.086602540378444})),
              {0.39, 0.95, 0.39, 0.95, 0.39, 0.95, 0.39, 0.95, 0.39, 0.95, 0.39, 0.95},
              {-0.025561381023353, 0.086293776138137, 0.12, 0.025561381023353, 0.086293776138137, 0.12, 0.087513292835791, -0.021010082747031, 0.12, 0.061951911812438, -0.065283693391106, 0.12, -0.061951911812438, -0.065283693391106, 0.12, -0.087513292835791, -0.021010082747032, 0.12}) {
    }

    // Avoids copying all fields per hand and uses the constructor instead.
    ParallelKinematicMachine6PUPS::ParallelKinematicMachine6PUPS(
        const ParallelKinematicMachine6PUPS& parallelKinematicMachine6PUPS)
        : ParallelKinematicMachine6PUPS(parallelKinematicMachine6PUPS.redundantJoints_, parallelKinematicMachine6PUPS.middleJointLimitations_, parallelKinematicMachine6PUPS.relativeEndEffectorJoints_) {
    }

    ParallelKinematicMachine6PUPS::ParallelKinematicMachine6PUPS(
        const arma::Cube<double>::fixed<3, 6, 2>& redundantJoints,
        const arma::Mat<double>::fixed<2, 6>& middleJointLimitations,
        const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints)
        // Linkages with an equal redundant joint start and end position does not have a redundant joint.
        : MachineModel(6, arma::accu(arma::any(redundantJoints.slice(1) - redundantJoints.slice(0)))),
          redundantJoints_(redundantJoints),
          middleJointLimitations_(middleJointLimitations),
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

        arma::Cube<double>::fixed<3, 6, 2> model;

        // The base joints
        model.slice(0) = redundantJoints_.slice(0);
        // Calculates the actual base joint position, in case the base joint is linked with a redundant joints.
        for (arma::uword n = 0; n < numberOfRedundantJoints_; ++n) {
          const arma::uword redundantJointIndex = redundantJointIndicies_(n);
          model.slice(0).col(redundantJointIndex) += redundantJointActuations(n) * redundantJointDirections_.col(redundantJointIndex);
        }
        
        // The end-effector joints
        // Corrects the orientation of the end-effector ...
        model.slice(1) = rotationMatrix3D(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * relativeEndEffectorJoints_;
        // ... and translate it into the *base* coordinate system.
        model.slice(1).each_col() += endEffectorPosition;
        
        // At this point, the model might be invalid, since some prismatic joints might be extended to far or to close, than allowed by the robot configuration (i.e. *middleJointLimitations_*).
        // However, in favour of the optimisation process, we decided to refrain from raising an exception at this point.
        
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
        const arma::Cube<double>::fixed<3, 6, 2>& model = getModel(endEffectorPose, redundantJointActuations);
        const arma::Mat<double>::fixed<3, 6>& baseJoints = model.slice(0);
        const arma::Mat<double>::fixed<3, 6>& endEffectorJoints = model.slice(1);
        
        const arma::Row<double>::fixed<6>& prismaticJointLengths = arma::sqrt(arma::sum(arma::square(model.slice(1) - model.slice(0))));
        if (arma::any(middleJointLimitations_.row(0) > prismaticJointLengths || prismaticJointLengths > middleJointLimitations_.row(1))) {
          // Based on the calculation below (excluding this condition), the worst/maximal objective value would be 1. To give the optimisation algorithm some insight in which direction to go for a valid robot model, we add the deviations from the allowed prismatic joints length (including both, the upper and lower bound) to the worst/maximal objective value.
          // This extends the procedure described an the aforementioned paper.
          return 1.0 + arma::accu(arma::max(arma::max(arma::zeros<arma::Row<double>>(numberOfActiveJoints_), middleJointLimitations_.row(0) - prismaticJointLengths), prismaticJointLengths - middleJointLimitations_.row(1)));
        }
        
        // Some helper variables, makes the following easier to read.
        const arma::Mat<double>::fixed<3, 6>& baseToEndEffectorJoints = endEffectorJoints - baseJoints;
        // Reverses the translation into the *base* coordinate system.
        const arma::Mat<double>::fixed<3, 6>& rotatedRelativeEndEffectorJoints = endEffectorJoints.each_col() - endEffectorPosition;
        
        // The forward kinematic 
        // *Note:* We fill the transpose of the forward kinematic, as this fits better with the shape of other matrices. This matrix is then later on transposed to get the actually intended one.
        arma::Mat<double>::fixed<6, 6> forwardKinematic;
        forwardKinematic.head_rows(3) = baseToEndEffectorJoints;
        for (arma::uword n = 0; n < forwardKinematic.n_rows; ++n) {
          forwardKinematic.submat(3, n, 5, n) = arma::cross(rotatedRelativeEndEffectorJoints.col(n), baseToEndEffectorJoints.col(n));
        }

        // The inverse kinematic
        arma::Mat<double> inverseKinematic(6, 6 + redundantJointIndicies_.n_elem, arma::fill::zeros);
        inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJoints)));
        
        for (arma::uword n = 0; n < numberOfRedundantJoints_; ++n) {
          const arma::uword redundantJointIndex = redundantJointIndicies_(n);
          inverseKinematic(redundantJointIndex, 6 + n) = arma::dot(baseToEndEffectorJoints.col(redundantJointIndex), redundantJointAngles_.col(n));
        }

        // Shifts the actual objective value (the negative inverse of the condition number) by 1, to let 0 be the best/lowest possible objective value.
        return 1.0 - 1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
      });
    }

    arma::Mat<double> ParallelKinematicMachine6PUPS::getRedundantJointAngles(
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
