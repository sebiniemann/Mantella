/*******************************************************************************
 * Copyright (c) 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
 *
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/MIT
 *******************************************************************************/
#pragma once

#include <armadillo>
using arma::Mat;
using arma::Col;
using arma::Row;
using arma::uword;
using arma::find;
using arma::any;
using arma::acos;
using arma::sum;
using arma::square;
using arma::prod;
using arma::cos;
using arma::sin;
using arma::norm;
using arma::solve;

#include <optimisation/problem/robotic/mechanism/Robot.cpp>
using optimisation::problem::robotic::mechanism::Robot;

#include <optimisation/util/algebra.cpp>
using optimisation::util::algebra::normMat;

#include <optimisation/util/geometry.cpp>
using optimisation::util::geometry::get2DRotationMatrix;

#include <optimisation/util/trigonometry.cpp>
using optimisation::util::trigonometry::atan2;

/**
 * A {@code n}(P̲)R̲RR robot, with {@code n} as the number of kinematic chains.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 * 
 * @see "Jens Kotlarski et al.: Influence of Kinematic Redundancy on the Singularity-free Workspace of Parallel Kinematic Machines, Frontiers of Mechanical Engineering, Vol. 7(2), pp. 120-134, 2012"
 */
namespace optimisation {
  namespace problem {
    namespace robotic {
      namespace mechanism {
        class Robot_NPRRR: public Robot {
          protected:
            /**
             * The XY-position of the end effector joint for each kinematic chain per row.
             */
            Mat<double> _endEffectorJointXYPosition;

            /**
             * The mechanical link length starting from the base joint to the end effector joint for each kinematic chain per row.
             */
            Mat<double> _linkLengths;

            /**
             * The XY-position of the start of the prismatic joint for each kinematic chain per row.
             */
            Mat<double> _prismaticJointStartXYPosition;

            /**
             * The XY-distance between the start and the end of the prismatic joint for each kinematic chain per row.
             */
            Mat<double> _prismaticJointXYDistance;

            /**
             * The indices of kinematic chains with redundant prismatic joints
             */
            Mat<uword> _kinematicChainsWithRedundancy;

            // Pre-allocated memory

            /**
             * The Jacobian
             */
            Mat<double> _jacobian;

            /**
             * The direct Jacobian
             */
            Mat<double> _directJacobian;

            /**
             * The inverse Jacobian
             */
            Mat<double> _inverseJacobian;

            /**
             * Used to specify the parameter for an actual {@code n}(P̲)R̲RR robot.
             * <p>
             * To denote that an optional prismatic joint is not present in the actual robot configuration for a kinematic chain,
             * set the XY-position of the start of the prismatic joint equal to its end for its corresponding column.
             * <p>
             * <b>Important note:</b> Some parameter must be relative to the origin of the base, while others must be relative to
             * the end effector.
             *
             * @param endEffectorJointXYPosition The XY-position of the end effector joint for each kinematic chain per row.
             *          <b>Note:</b> Must be relative to the origin of the end effector.
             * @param linkLengths The mechanical link length starting from the base joint to the end effector joint for each
             *          kinematic chain per row.
             * @param prismaticJointStartXYPosition The XY-position of the start of the prismatic joint for each kinematic chain
             *          per row. <b>Note:</b> Must be relative to the origin of the base.
             * @param prismaticJointEndXYPosition The XY-positions of the end of the prismatic joint for each kinematic chain per
             *          row. <b>Note:</b> Must be relative to the origin of the base.
             */
            Robot_NPRRR(Mat<double> endEffectorJointXYPosition,
                Mat<double> linkLengths,
                Mat<double> prismaticJointStartXYPosition,
                Mat<double> prismaticJointEndXYPosition) {
              endEffectorJointXYPosition.reshape(3, 2);
              linkLengths.reshape(3, 2);
              prismaticJointStartXYPosition.reshape(3, 2);
              prismaticJointEndXYPosition.reshape(3, 2);

              _endEffectorJointXYPosition = endEffectorJointXYPosition;
              _linkLengths = linkLengths;
              _prismaticJointStartXYPosition = prismaticJointStartXYPosition;

              _prismaticJointXYDistance = prismaticJointEndXYPosition - _prismaticJointStartXYPosition;

              // Pre-allocated memory
              _kinematicChainsWithRedundancy = find(normMat(_prismaticJointXYDistance, 2, 1));
              int numberOfKinematicChains = _endEffectorJointXYPosition.n_rows;
              int numberOfKinematicChainsWithRedundancy = _kinematicChainsWithRedundancy.n_elem;

              _jacobian.set_size(3, numberOfKinematicChains + numberOfKinematicChainsWithRedundancy);
              _directJacobian.set_size(numberOfKinematicChains, 3);
              _inverseJacobian.set_size(numberOfKinematicChains, numberOfKinematicChains + numberOfKinematicChainsWithRedundancy);
            }

          public:
            bool isValidConfiguration(const Mat<double>& prismaticJointRelativeDistances, const Row<
                double>& XYPosition, double rollAngle, double pitchAngle, double yawAngle)
                override {
              // Rotate the end effector joint positions if the end effector was rotated.
              Mat<double> rotatedEndEffectorJointXYPosition(_endEffectorJointXYPosition);
              if (yawAngle != 0) {
                Mat<double> rotationMatrix = get2DRotationMatrix(yawAngle);

                for (int i = 0; i < rotatedEndEffectorJointXYPosition.n_rows;
                    i++) {
                  rotatedEndEffectorJointXYPosition.row(i) = rotatedEndEffectorJointXYPosition.row(i) * rotationMatrix;
                }
              }
              rotatedEndEffectorJointXYPosition.each_row() += XYPosition;

              Mat<double> baseJointToEndEffectorJointXYPosition = rotatedEndEffectorJointXYPosition - (_prismaticJointStartXYPosition + _prismaticJointXYDistance % prismaticJointRelativeDistances);
              if (any((Col<uword>) (normMat(baseJointToEndEffectorJointXYPosition, 2, 1) > sum(_linkLengths, 1)))) {
                return false;
              }

              // See the paper cited in the class description for more information about the exact position of the angle (called
              // psi within the paper).
              Mat<double> passiveJointAngles = acos((sum(square(baseJointToEndEffectorJointXYPosition), 1) - (sum(square(_linkLengths), 1))) / (prod(_linkLengths, 1) * 2));
              if (!passiveJointAngles.is_finite()) {
                return false;
              }

              return true;
            }

            Mat<double> getJacobian(const Mat<double>& prismaticJointRelativeDistances, const Row<
                double>& XYPosition, double rollAngle, double pitchAngle, double yawAngle)
                override {
              // Rotate the end effector joint positions if the end effector was rotated.
              Mat<double> rotatedEndEffectorJointXYPosition(_endEffectorJointXYPosition);
              if (yawAngle != 0) {
                Mat<double> rotationMatrix = get2DRotationMatrix(yawAngle);

                for (int i = 0; i < rotatedEndEffectorJointXYPosition.n_rows;
                    i++) {
                  rotatedEndEffectorJointXYPosition.row(i) = rotatedEndEffectorJointXYPosition.row(i) * rotationMatrix;
                }
              }

              // The XY-position of the end effector joint for each kinematic chain per row, relative to the origin of the base.
              // A copy is made since the XY-Position relative to the orign of the end effector is later needed to calculate the
              // direct Jacobian.
              Mat<double> rotatedAndShiftedEndEffectorJointXYPosition(rotatedEndEffectorJointXYPosition);
              rotatedAndShiftedEndEffectorJointXYPosition.each_row() += XYPosition;

              Mat<double> baseJointToEndEffectorJointXYPosition = rotatedAndShiftedEndEffectorJointXYPosition - (_prismaticJointStartXYPosition + _prismaticJointXYDistance % prismaticJointRelativeDistances);
              // See the paper cited in the class description for more information about the exact position of the angle (called
              // psi within the paper).
              Mat<double> passiveJointAngles = acos((sum(square(baseJointToEndEffectorJointXYPosition), 1) - (sum(square(_linkLengths), 1))) / (prod(_linkLengths, 1) * 2));
              // See the paper cited in the class description for more information about the exact position of the angle (called
              // theta within the paper).
              Mat<double> activeJointAngles = atan2(
                  (_linkLengths.col(0) + _linkLengths.col(1) % cos(passiveJointAngles)) % baseJointToEndEffectorJointXYPosition.col(1) - (_linkLengths.col(1) % sin(passiveJointAngles)) % baseJointToEndEffectorJointXYPosition.col(0),
                  (_linkLengths.col(0) + _linkLengths.col(1) % cos(passiveJointAngles)) % baseJointToEndEffectorJointXYPosition.col(0) + (_linkLengths.col(1) % sin(passiveJointAngles)) % baseJointToEndEffectorJointXYPosition.col(1)
                      );

              _directJacobian.col(0) = baseJointToEndEffectorJointXYPosition.col(0) - _linkLengths.col(0) % cos(activeJointAngles);
              _directJacobian.col(1) = baseJointToEndEffectorJointXYPosition.col(1) - _linkLengths.col(0) % sin(activeJointAngles);
              _directJacobian.col(2) = _directJacobian.col(1) % rotatedEndEffectorJointXYPosition.col(0) - _directJacobian.col(0) % rotatedEndEffectorJointXYPosition.col(1);

              _inverseJacobian.diag() = _linkLengths.col(0) % (_directJacobian.col(0) % sin(activeJointAngles) - _directJacobian.col(1) % cos(activeJointAngles));
              for (int n = 0; n < _kinematicChainsWithRedundancy.n_elem; n++) {
                _inverseJacobian.at(n, _endEffectorJointXYPosition.n_rows + n) = (_directJacobian.at(n, 0) * _prismaticJointXYDistance.at(n, 0) + _directJacobian.at(n, 1) * _prismaticJointXYDistance.at(n, 1)) / norm(_prismaticJointXYDistance.row(n), 2);
              }

              solve(_jacobian, _directJacobian, _inverseJacobian);
              return _jacobian;
            }
        };
      }
    }
  }
}
