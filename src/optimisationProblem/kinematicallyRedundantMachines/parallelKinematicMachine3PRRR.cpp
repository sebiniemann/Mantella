#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine3PRRR.hpp"

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
    ParallelKinematicMachine3PRRR::ParallelKinematicMachine3PRRR()
        : KinematicallyRedundantMachines(3, 3),
          redundantJointsPosition_(
              {{0.6, 0.0, 1.2},
               {1.039230484541327, 0.0, 0.0}}),
          redundantJointsAngles_(
              {{0.0, -1.0, -1.0},
               {1.0, 0.0, 0.0}}),
          linkLengths_(
              {{0.6, 0.6, 0.6},
               {0.6, 0.6, 0.6}}),
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

                arma::mat::fixed<2, 3> middleJointsPosition;
                for (arma::uword k = 0; k < baseJointsPosition.n_cols; ++k) {
                  const std::vector<arma::vec::fixed<2>>& intersections = circleCircleIntersections(baseJointsPosition.col(k), linkLengths_(0, k), endEffectorJointsPosition.col(k), linkLengths_(1, k));

                  if (intersections.size() > 1) {
                    middleJointsPosition.col(k) = intersections.at(0);
                  } else {
                    return std::numeric_limits<decltype(poseInaccuracy)>::infinity();
                  }
                }

                const arma::mat::fixed<2, 3>& baseToMiddleJointsPosition = middleJointsPosition - baseJointsPosition;
                const arma::mat::fixed<2, 3>& middleToEndEffectorJointsPosition = endEffectorJointsPosition - middleJointsPosition;
                const arma::mat::fixed<2, 3>& endEffectorJointsRotatedPosition = endEffectorJointsPosition.each_col() - endEffectorPosition;

                arma::mat::fixed<3, 3> forwardKinematic;
                forwardKinematic.head_rows(2) = middleToEndEffectorJointsPosition;
                forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointsRotatedPosition.row(1) + forwardKinematic.row(1) % endEffectorJointsRotatedPosition.row(0);

                arma::mat::fixed<3, 6> inverseKinematic(arma::fill::zeros);
                inverseKinematic.diag() = forwardKinematic.row(0) % baseToMiddleJointsPosition.row(1) - forwardKinematic.row(1) % baseToMiddleJointsPosition.row(0);
                for (arma::uword k = 0; k < redundantJointsActuation_.n_elem; ++k) {
                  inverseKinematic(k, 3 + k) = -arma::dot(middleToEndEffectorJointsPosition.col(k), redundantJointsAngles_.col(k));
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
            "KRM Parallel Kinematic Machine 3PRRR"}});
    }
  }
}
