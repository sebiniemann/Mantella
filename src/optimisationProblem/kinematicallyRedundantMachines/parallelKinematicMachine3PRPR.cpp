#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine3PRPR.hpp"

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
    ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR()
        : KinematicallyRedundantMachines(3, 3),
          redundantJointsPosition_(
              {{0.6, 0.0, 1.2},
               {1.039230484541327, 0.0, 0.0}}),
          redundantJointsAngles_(
              {{0.0, -1.0, -1.0},
               {1.0, 0.0, 0.0}}),
          middleJointsMinimalLength_(
              {0.1, 0.1, 0.1}),
          middleJointsMaximalLength_(
              {1.2, 1.2, 1.2}),
          endEffectorJointsRelativePosition_(
              {{-0.000066580445834, -0.092751709777083, 0.092818290222917},
               {0.106954081945581, -0.053477040972790, -0.053477040972790}}) {
      setLowerBounds({-0.5, -0.2, -0.2});
      setUpperBounds({0.5, 0.8, 0.8});

      setObjectiveFunctions(
          {{[this](
                const arma::vec& redundantJointsActuation_) {
              assert(redundantJointsActuation_.n_elem == numberOfDimensions_);

              double poseInaccuracy = 0.0;
              for (arma::uword n = 0; n < endEffectorTrajectory_.n_cols; ++n) {
                const arma::vec::fixed<3>& endEffectorPose = endEffectorTrajectory_.col(n);
                const arma::vec::fixed<2>& endEffectorPosition = endEffectorPose.head(2);
                const double endEffectorAngle = endEffectorPose(2);

                arma::mat::fixed<2, 3> baseJointsPosition = redundantJointsPosition_;
                for (arma::uword k = 0; k < redundantJointsActuation_.n_elem; ++k) {
                  baseJointsPosition.col(k) += redundantJointsActuation_(k) * redundantJointsAngles_.col(k);
                }

                arma::mat::fixed<2, 3> endEffectorJointsPosition = rotationMatrix2d(endEffectorAngle) * endEffectorJointsRelativePosition_;
                endEffectorJointsPosition.each_col() += endEffectorPosition;

                const arma::rowvec::fixed<3>& middleJointsLength = arma::sqrt(arma::sum(arma::square(endEffectorJointsPosition - baseJointsPosition)));
                if (arma::any(middleJointsMinimalLength_ > middleJointsLength || middleJointsLength > middleJointsMaximalLength_)) {
                  return std::numeric_limits<decltype(poseInaccuracy)>::infinity();
                }

                const arma::mat::fixed<2, 3>& baseToEndEffectorJointsPosition = endEffectorJointsPosition - baseJointsPosition;
                const arma::mat::fixed<2, 3>& endEffectorJointsRotatedPosition = endEffectorJointsPosition.each_col() - endEffectorPosition;

                arma::mat::fixed<3, 3> forwardKinematic;
                forwardKinematic.head_rows(2) = baseToEndEffectorJointsPosition;
                forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointsRotatedPosition.row(1) + forwardKinematic.row(1) % endEffectorJointsRotatedPosition.row(0);

                arma::mat::fixed<3, 6> inverseKinematic(arma::fill::zeros);
                inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointsPosition)));
                for (arma::uword k = 0; k < redundantJointsActuation_.n_elem; ++k) {
                  inverseKinematic(k, 3 + k) = arma::dot(baseToEndEffectorJointsPosition.col(k), redundantJointsAngles_.col(k));
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
            "KRM Parallel Kinematic Machine 3PRPR"}});
    }
  }
}
