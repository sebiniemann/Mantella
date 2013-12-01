/*******************************************************************************
 * Copyright (c) 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
 *
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/MIT
 *******************************************************************************/
package optimisation.problem.robotic.mechanism;

import optimisation.util.Algebra;
import optimisation.util.Geometry;
import optimisation.util.Sparse;
import arma.Arma;
import arma.Mat;
import arma.Op;

/**
 * A {@code n}(P̲)UP̲S robot, with {@code n} as the number of kinematic chains.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 * 
 * @see "Jens Kotlarski et al.: Influence of Kinematic Redundancy on the Singularity-free Workspace of Parallel Kinematic Machines, Frontiers of Mechanical Engineering, Vol. 7(2), pp. 120-134, 2012"
 */
public class Robot_NPUPS extends Robot {

  /**
   * The XY-position of the end effector joint for each kinematic chain per row.
   */
  protected Mat _endEffectorJointXYZPosition;

  /**
   * The XY-position of the start of the prismatic joint for each kinematic chain per row.
   */
  protected Mat _prismaticJointStartXYZPosition;

  /**
   * The XY-distance between the start and the end of the prismatic joint for each kinematic chain per row.
   */
  protected Mat _prismaticJointXYZDistance;

  // Pre-allocated memory

  /**
   * The Jacobian
   */
  protected Mat _jacobian;

  /**
   * The direct Jacobian
   */
  protected Mat _directJacobian;

  /**
   * The inverse Jacobian
   */
  protected Mat _inverseJacobian;

  /**
   * Used to specify the parameter for an actual {@code n}(P̲)R̲RR robot.
   * <p>
   * To denote that an optional prismatic joint is not present in the actual robot configuration for a kinematic chain,
   * set the XY-position of the start of the prismatic joint equal to its end for its corresponding column.
   * <p>
   * <b>Important note:</b> Some parameter must be relative to the origin of the base, while others must be relative to
   * the end effector.
   * 
   * @param endEffectorJointXYZPosition The XYZ-position of the end effector joint for each kinematic chain per row.
   *          <b>Note:</b> Must be relative to the origin of the end effector.
   * @param prismaticJointStartXYZPosition The XYZ-position of the start of the prismatic joint for each kinematic chain
   *          per row. <b>Note:</b> Must be relative to the origin of the base.
   * @param prismaticJointEndXYZPosition The XYZ-positions of the end of the prismatic joint for each kinematic chain
   *          per
   *          row. <b>Note:</b> Must be relative to the origin of the base.
   */
  protected Robot_NPUPS(Mat endEffectorJointXYZPosition,
                        Mat prismaticJointStartXYZPosition,
                        Mat prismaticJointEndXYZPosition) {
    _endEffectorJointXYZPosition = endEffectorJointXYZPosition;
    _prismaticJointStartXYZPosition = prismaticJointStartXYZPosition;

    _prismaticJointXYZDistance = _prismaticJointStartXYZPosition.minus(prismaticJointStartXYZPosition);

    // Pre-allocated memory
    int numberOfKinematicChains = _endEffectorJointXYZPosition.n_rows;

    _jacobian = new Mat(6, numberOfKinematicChains + Sparse.nnz(_prismaticJointXYZDistance));
    _directJacobian = new Mat(numberOfKinematicChains, 6);
    _inverseJacobian = new Mat(numberOfKinematicChains, numberOfKinematicChains + Sparse.nnz(_prismaticJointXYZDistance));
  }

  @Override
  public Mat getJacobian(Mat prismaticJointRelativeDistances, Mat XYZposition, double rollAngle, double pitchAngle, double yawAngle) {
    // Rotate the end effector joint positions if the end effector was rotated.
    Mat rotatedEndEffectorJointXYZPosition = new Mat(_endEffectorJointXYZPosition);
    if (rollAngle != 0 || pitchAngle != 0 || yawAngle != 0) {
      Mat rotationMatrix = Geometry.getZYXTaitBryanAnglesRotationMatrix(rollAngle, pitchAngle, yawAngle);

      for (int i = 0; i < rotatedEndEffectorJointXYZPosition.n_rows; i++) {
        rotatedEndEffectorJointXYZPosition.row(i, Op.EQUAL, rotatedEndEffectorJointXYZPosition.row(i).times(rotationMatrix));
      }
    }

    // The XY-position of the end effector joint for each kinematic chain per row, relative to the origin of the base.
    // A copy is made since the XY-Position relative to the orign of the end effector is later needed to calculate the
    // direct Jacobian.
    Mat rotatedAndShiftedEndEffectorJointXYZPosition = new Mat(rotatedEndEffectorJointXYZPosition);
    rotatedAndShiftedEndEffectorJointXYZPosition.each_row(Op.PLUS, XYZposition);

    Mat baseJointToEndEffectorJointXYZPosition = rotatedAndShiftedEndEffectorJointXYZPosition.minus(_prismaticJointStartXYZPosition.plus(_prismaticJointXYZDistance.elemTimes(prismaticJointRelativeDistances)));

    _directJacobian.cols(0, 2, Op.EQUAL, baseJointToEndEffectorJointXYZPosition);
    _directJacobian.cols(3, 5, Op.EQUAL, Algebra.crossMat(rotatedEndEffectorJointXYZPosition, baseJointToEndEffectorJointXYZPosition, 1));

    _inverseJacobian.diag(Op.EQUAL, Algebra.normMat(baseJointToEndEffectorJointXYZPosition, 2, 1));

    System.out.println(_directJacobian);
    System.out.println(_inverseJacobian);

    Arma.solve(_jacobian, _directJacobian, _inverseJacobian);
    return _jacobian;
  }

}
