#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/parallelKinematicMachine3PRPR.hpp"

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
    ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR()
        : ParallelKinematicMachine3PRPR(
              arma::join_slices(arma::Mat<double>::fixed<2, 3>({0.1, 1.0392, 0.0, 0.8, 1.2, 0.8}), arma::Mat<double>::fixed<2, 3>({1.1, 1.0392, 0.0, -0.2, 1.2, -0.2})),
              {0.1, 1.2, 0.1, 1.2, 0.1, 1.2},
              {-0.000066580445834, 0.106954081945581, -0.092751709777083, -0.053477040972790, 0.092818290222917, -0.053477040972790}) {
    }

    // Avoids copying all fields per hand and uses the constructor instead.
    ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR(
        const ParallelKinematicMachine3PRPR& parallelKinematicMachine3PRPR)
        : ParallelKinematicMachine3PRPR(parallelKinematicMachine3PRPR.redundantJoints_, parallelKinematicMachine3PRPR.middleJointLimitations_, parallelKinematicMachine3PRPR.relativeEndEffectorJoints_) {
    }

    ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR(
        const arma::Cube<double>::fixed<2, 3, 2>& redundantJoints,
        const arma::Mat<double>::fixed<2, 3>& middleJointLimitations,
        const arma::Mat<double>::fixed<2, 3>& relativeEndEffectorJoints)
        // Linkages with an equal redundant joint start and end position does not have a redundant joint.
        : MachineModel(3, arma::accu(arma::any(redundantJoints.slice(1) - redundantJoints.slice(0)))),
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
        const arma::Col<double>::fixed<2>& endEffectorPosition = endEffectorPose.head(2);
        const double endEffectorAngle = endEffectorPose(2);

        arma::Cube<double>::fixed<2, 3, 2> model;

        // The base joints
        model.slice(0) = redundantJoints_.slice(0);
        // Calculates the actual base joint position, in case the base joint is linked with a redundant joints.
        for (arma::uword n = 0; n < numberOfRedundantJoints_; ++n) {
          const arma::uword redundantJointIndex = redundantJointIndicies_(n);
          model.slice(0).col(redundantJointIndex) += redundantJointActuations(n) * redundantJointDirections_.col(redundantJointIndex);
        }
        
        // The end-effector joints
        // Corrects the orientation of the end-effector ...
        model.slice(1) = rotationMatrix2D(endEffectorAngle) * relativeEndEffectorJoints_;
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
        const arma::Col<double>::fixed<2>& endEffectorPosition = endEffectorPose.head(2);
        
        // The robots model
        const arma::Cube<double>::fixed<2, 3, 2>& model = getModel(endEffectorPose, redundantJointActuations);
        const arma::Mat<double>::fixed<2, 3>& baseJoints = model.slice(0);
        const arma::Mat<double>::fixed<2, 3>& endEffectorJoints = model.slice(1);
        
        const arma::Row<double>::fixed<3>& prismaticJointLengths = arma::sqrt(arma::sum(arma::square(model.slice(1) - model.slice(0))));
        if (arma::any(middleJointLimitations_.row(0) > prismaticJointLengths || prismaticJointLengths > middleJointLimitations_.row(1))) {
          // Based on the calculation below (excluding this condition), the worst/maximal objective value would be 1. To give the optimisation algorithm some insight in which direction to go for a valid robot model, we add the deviations from the allowed prismatic joints length (including both, the upper and lower bound) to the worst/maximal objective value.
          // This extends the procedure described an the aforementioned paper.
          return 1.0 + arma::accu(arma::max(arma::max(arma::zeros<arma::Row<double>>(numberOfActiveJoints_), middleJointLimitations_.row(0) - prismaticJointLengths), prismaticJointLengths - middleJointLimitations_.row(1)));
        }
        
        // Some helper variables, makes the following easier to read.
        const arma::Mat<double>::fixed<2, 3>& baseToEndEffectorJoints = endEffectorJoints - baseJoints;
        // Reverses the translation into the *base* coordinate system.
        const arma::Mat<double>::fixed<2, 3>& rotatedRelativeEndEffectorJoints = endEffectorJoints.each_col() - endEffectorPosition;

        // The forward kinematic 
        // *Note:* We fill the transpose of the forward kinematic, as this fits better with the shape of other matrices. This matrix is then later on transposed to get the actually intended one.
        arma::Mat<double>::fixed<3, 3> forwardKinematic;
        forwardKinematic.head_rows(2) = baseToEndEffectorJoints;
        forwardKinematic.row(2) = -forwardKinematic.row(0) % rotatedRelativeEndEffectorJoints.row(1) + forwardKinematic.row(1) % rotatedRelativeEndEffectorJoints.row(0);

        // The inverse kinematic
        arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
        inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJoints)));
        
        for (arma::uword n = 0; n < numberOfRedundantJoints_; ++n) {
          const arma::uword redundantJointIndex = redundantJointIndicies_(n);
          inverseKinematic(redundantJointIndex, 3 + n) = arma::dot(baseToEndEffectorJoints.col(redundantJointIndex), redundantJointAngles_.col(n));
        }

        // Shifts the actual objective value (the negative inverse of the condition number) by 1, to let 0 be the best/lowest possible objective value.
        return 1.0 - 1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
      });
    }

    arma::Mat<double> ParallelKinematicMachine3PRPR::getRedundantJointAngles(
        const arma::Mat<double>::fixed<2, 3> redundantJointDirections,
        const arma::Col<arma::uword> redundantJointIndicies) const {
      arma::Mat<double> redundantJointAngles(2, redundantJointIndicies.n_elem);

      for (arma::uword n = 0; n < numberOfRedundantJoints_; ++n) {
        const arma::uword redundantJointIndex = redundantJointIndicies_(n);

        const double redundantJointAngle = std::atan2(redundantJointDirections(1, redundantJointIndex), redundantJointDirections(0, redundantJointIndex));
        redundantJointAngles(0, n) = std::cos(redundantJointAngle);
        redundantJointAngles(1, n) = std::sin(redundantJointAngle);
      }

      return redundantJointAngles;
    }
  }
}
