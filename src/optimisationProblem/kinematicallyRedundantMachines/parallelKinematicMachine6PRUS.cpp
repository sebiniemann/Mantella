#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine6PRUS.hpp"

// C++ standard library
#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <string>
#include <utility>
#include <vector>
// IWYU pragma: no_include <ext/alloc_traits.h>

// Mantella
#include "mantella_bits/geometry.hpp"

// The following robot configurations and calculations are based on the work of our research colleagues from the Institute of Mechatronic Systems, Leibniz Universität Hannover, Germany.
// To understand the following equations, we advise to carefully read the following paper:
// @see J. Kotlarski et al. (2012). Influence of kinematic redundancy on the singularity-free workspace of parallel kinematic machines. Frontiers of Mechanical Engineering, 7(2), pp. 120-134.
namespace mant {
  namespace krm {
    ParallelKinematicMachine6PRUS::ParallelKinematicMachine6PRUS()
        : KinematicallyRedundantMachines(6, 6),
          redundantJointsPosition_(
              {{-0.050659008749464, 0.050659008749464, 0.337494923062311, 0.286835914312847, -0.286835914312847, -0.337494923062311},
               {0.360457577021932, 0.360457577021932, -0.136356800003392, -0.224100777018540, -0.224100777018540, -0.136356800003392},
               {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}),
          redundantJointsAngles_(
              {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
               {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
               {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}}),
          baseJointsNormal_(
              {{0.990268068741570, 0.990268068741570, -0.374606593415912, -0.615661475325659, -0.615661475325659, -0.374606593415912},
               {0.139173100960066, -0.139173100960066, -0.927183854566787, -0.788010753606722, 0.788010753606722, 0.927183854566787},
               {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}),
          linkLengths_(
              {{0.24, 0.24, 0.24, 0.24, 0.24, 0.24},
               {0.56, 0.56, 0.56, 0.56, 0.56, 0.56}}),
          endEffectorJointsRelativePosition_(
              {{-0.027346281319362, 0.027346281319362, 0.072289569018135, 0.042908323417351, -0.044943287698773, -0.072289569018135},
               {0.067684421383375, 0.067684421383375, -0.010159636370085, -0.059058240589371, -0.057524785013291, -0.010159636370085},
               {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}) {
      setLowerBounds(arma::zeros<arma::vec>(numberOfDimensions_) - 0.6);
      setUpperBounds(arma::zeros<arma::vec>(numberOfDimensions_) + 0.2);

      setObjectiveFunctions(
          {{[this](
                const arma::vec& redundantJointsActuation_) {
              assert(redundantJointsActuation_.n_elem == numberOfDimensions_);

              double poseInaccuracy = 0.0;
              for (arma::uword n = 0; n < endEffectorTrajectory_.n_cols; ++n) {
                const arma::vec::fixed<6>& endEffectorPose = endEffectorTrajectory_.col(n);
                const arma::vec::fixed<3>& endEffectorPosition = endEffectorPose.head(3);
                const double endEffectorRollAngle = endEffectorPose(3);
                const double endEffectorPitchAngle = endEffectorPose(4);
                const double endEffectorYawAngle = endEffectorPose(5);

                arma::mat::fixed<3, 6> baseJointsPosition = redundantJointsPosition_;
                for (arma::uword k = 0; k < redundantJointsActuation_.n_elem; ++k) {
                  baseJointsPosition.col(k) += redundantJointsActuation_(k) * redundantJointsAngles_.col(k);
                }

                arma::mat::fixed<3, 6> endEffectorJointsPosition = rotationMatrix3d(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointsRelativePosition_;
                endEffectorJointsPosition.each_col() += endEffectorPosition;

                arma::mat::fixed<3, 6> middleJointsPosition;
                for (arma::uword k = 0; k < baseJointsPosition.n_cols; ++k) {
                  const std::vector<arma::vec::fixed<3>>& intersections = circleSphereIntersections(
                      baseJointsPosition.col(k), linkLengths_(0, k), baseJointsNormal_.col(k), endEffectorJointsPosition.col(k), linkLengths_(1, k));

                  if (intersections.size() > 1) {
                    middleJointsPosition.col(k) = intersections.at(0);
                  } else {
                    return std::numeric_limits<decltype(poseInaccuracy)>::infinity();
                  }
                }

                const arma::mat::fixed<3, 6>& baseToMiddleJointsPosition = middleJointsPosition - baseJointsPosition;
                const arma::mat::fixed<3, 6>& middleToEndEffectorJointsPosition = endEffectorJointsPosition - middleJointsPosition;
                const arma::mat::fixed<3, 6>& baseToEndEffectorJointsPosition = endEffectorJointsPosition - baseJointsPosition;
                const arma::mat::fixed<3, 6>& endEffectorJointsRotatedPosition = endEffectorJointsPosition.each_col() - endEffectorPosition;

                arma::mat::fixed<6, 6> forwardKinematic;
                forwardKinematic.head_rows(3) = middleToEndEffectorJointsPosition;
                for (arma::uword k = 0; k < forwardKinematic.n_rows; ++k) {
                  forwardKinematic.submat(3, k, 5, k) = arma::cross(endEffectorJointsRotatedPosition.col(k), middleToEndEffectorJointsPosition.col(k));
                }

                arma::mat::fixed<6, 12> inverseKinematic(arma::fill::zeros);
                inverseKinematic.diag() = baseToEndEffectorJointsPosition.row(0) % baseToMiddleJointsPosition.row(1) - baseToEndEffectorJointsPosition.row(1) % baseToMiddleJointsPosition.row(0);
                for (arma::uword k = 0; k < redundantJointsActuation_.n_elem; ++k) {
                  inverseKinematic(k, 6 + k) = arma::dot(middleToEndEffectorJointsPosition.col(k), redundantJointsAngles_.col(k));
                }

                arma::mat solution;
                if (!arma::solve(solution, forwardKinematic.t(), inverseKinematic)) {
                  return std::numeric_limits<decltype(poseInaccuracy)>::infinity();
                } else {
                  try {
                    poseInaccuracy = std::max(poseInaccuracy, arma::cond(solution));
                  } catch (...) {
                    return std::numeric_limits<decltype(poseInaccuracy)>::infinity();
                  }
                }
              }

              return poseInaccuracy;
            },
            "KRM Parallel Kinematic Machine 6PRUS"}});
    }
  }
}
