#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine6PUPS.hpp"

// C++ standard library
#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <string>
#include <utility>

// Mantella
#include "mantella_bits/geometry.hpp"

// The following robot configurations and calculations are based on the work of our research colleagues from the Institute of Mechatronic Systems, Leibniz Universität Hannover, Germany.
// To understand the following equations, we advise to carefully read the following paper:
// @see J. Kotlarski et al. (2012). Influence of kinematic redundancy on the singularity-free workspace of parallel kinematic machines. Frontiers of Mechanical Engineering, 7(2), pp. 120-134.
namespace mant {
  namespace krm {
    ParallelKinematicMachine6PUPS::ParallelKinematicMachine6PUPS()
        : KinematicallyRedundantMachines(6, 6),
          redundantJointsPosition_(
              {{-0.304055915910215, 0.304055915910215, 0.415348105304299, 0.111292189394084, -0.111292189394084, -0.415348105304299},
               {0.304055915910215, 0.304055915910215, 0.111292189394084, -0.415348105304299, -0.415348105304299, 0.111292189394084},
               {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}),
          redundantJointsAngles_(
              {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
               {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
               {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}}),
          middleJointsMinimalLength_(
              {0.39, 0.39, 0.39, 0.39, 0.39, 0.39}),
          middleJointsMaximalLength_(
              {0.95, 0.95, 0.95, 0.95, 0.95, 0.95}),
          endEffectorJointsRelativePosition_(
              {{-0.023293714059227, 0.023293714059227, 0.086933324366016, 0.063639610306789, 0.086933324366016, -0.086933324366016},
               {0.086933324366016, 0.086933324366016, -0.023293714059227, 0.063639610306789, 0.023293714059227, -0.023293714059227},
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

                const arma::rowvec::fixed<6>& middleJointsLength = arma::sqrt(arma::sum(arma::square(endEffectorJointsPosition - baseJointsPosition)));
                if (arma::any(middleJointsMinimalLength_ > middleJointsLength || middleJointsLength > middleJointsMaximalLength_)) {
                  return std::numeric_limits<decltype(poseInaccuracy)>::infinity();
                }

                const arma::mat::fixed<3, 6>& baseToEndEffectorJointsPosition = endEffectorJointsPosition - baseJointsPosition;
                const arma::mat::fixed<3, 6>& endEffectorJointsRotatedPosition = endEffectorJointsPosition.each_col() - endEffectorPosition;

                arma::mat::fixed<6, 6> forwardKinematic;
                forwardKinematic.head_rows(3) = baseToEndEffectorJointsPosition;
                for (arma::uword k = 0; k < forwardKinematic.n_rows; ++k) {
                  forwardKinematic.submat(3, k, 5, k) = arma::cross(endEffectorJointsRotatedPosition.col(k), baseToEndEffectorJointsPosition.col(k));
                }

                arma::mat::fixed<6, 12> inverseKinematic(arma::fill::zeros);
                inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointsPosition)));
                for (arma::uword k = 0; k < redundantJointsActuation_.n_elem; ++k) {
                  inverseKinematic(k, 6 + k) = arma::dot(baseToEndEffectorJointsPosition.col(k), redundantJointsAngles_.col(k));
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
            "KRM Parallel Kinematic Machine 6PUPS"}});
    }
  }
}
