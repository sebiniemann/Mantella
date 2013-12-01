package optimisation.problem.robotic.mechanism;

import optimisation.util.Geometry;
import optimisation.util.Sparse;
import optimisation.util.Trigonometry;
import arma.Arma;
import arma.Mat;
import arma.Op;

/**
 * A {@code n}(P̲)R̲RR robot, with {@code n} as the number of kinematic chains.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 * 
 * @see "Jens Kotlarski et al.: Influence of Kinematic Redundancy on the Singularity-free Workspace of Parallel Kinematic Machines, Frontiers of Mechanical Engineering, Vol. 7(2), pp. 120-134, 2012"
 */
public abstract class Robot_NPRRR extends Robot {

  /**
   * The XY-position of the end effector joint for each kinematic chain per row.
   */
  protected Mat _endEffectorJointXYPosition;

  /**
   * The mechanical link length starting from the base joint to the end effector joint for each kinematic chain per row.
   */
  protected Mat _linkLengths;

  /**
   * The XY-position of the start of the prismatic joint for each kinematic chain per row.
   */
  protected Mat _prismaticJointStartXYPosition;

  /**
   * The XY-distance between the start and the end of the prismatic joint for each kinematic chain per row.
   */
  protected Mat _prismaticJointXYDistance;

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
   * @param endEffectorJointXYPosition The XY-position of the end effector joint for each kinematic chain per row.
   *          <b>Note:</b> Must be relative to the origin of the end effector.
   * @param linkLengths The mechanical link length starting from the base joint to the end effector joint for each
   *          kinematic chain per row.
   * @param prismaticJointStartXYPosition The XY-position of the start of the prismatic joint for each kinematic chain
   *          per row. <b>Note:</b> Must be relative to the origin of the base.
   * @param prismaticJointEndXYPosition The XY-positions of the end of the prismatic joint for each kinematic chain per
   *          row. <b>Note:</b> Must be relative to the origin of the base.
   */
  protected Robot_NPRRR(Mat endEffectorJointXYPosition,
                        Mat linkLengths,
                        Mat prismaticJointStartXYPosition,
                        Mat prismaticJointEndXYPosition) {
    _endEffectorJointXYPosition = endEffectorJointXYPosition;
    _linkLengths = linkLengths;
    _prismaticJointStartXYPosition = prismaticJointStartXYPosition;

    _prismaticJointXYDistance = _prismaticJointStartXYPosition.minus(prismaticJointStartXYPosition);

    // Pre-allocated memory
    int numberOfKinematicChains = _endEffectorJointXYPosition.n_rows;

    _jacobian = new Mat(3, numberOfKinematicChains + Sparse.nnz(_prismaticJointXYDistance));
    _directJacobian = new Mat(numberOfKinematicChains, 3);
    _inverseJacobian = new Mat(numberOfKinematicChains, numberOfKinematicChains + Sparse.nnz(_prismaticJointXYDistance));
  }

  @Override
  public Mat getJacobian(Mat prismaticJointRelativeDistances, Mat XYPosition, double rollAngle, double pitchAngle, double yawAngle) {
    // Rotate the end effector joint positions if the end effector was rotated.
    Mat rotatedEndEffectorJointXYPosition = new Mat(_endEffectorJointXYPosition);
    if (yawAngle != 0) {
      Mat rotationMatrix = Geometry.get2DRotationMatrix(yawAngle);

      for (int i = 0; i < rotatedEndEffectorJointXYPosition.n_rows; i++) {
        rotatedEndEffectorJointXYPosition.row(i, Op.EQUAL, rotatedEndEffectorJointXYPosition.row(i).times(rotationMatrix));
      }
    }

    // The XY-position of the end effector joint for each kinematic chain per row, relative to the origin of the base.
    // A copy is made since the XY-Position relative to the orign of the end effector is later needed to calculate the
    // direct Jacobian.
    Mat rotatedAndShiftedEndEffectorJointXYPosition = new Mat(rotatedEndEffectorJointXYPosition);
    rotatedEndEffectorJointXYPosition.each_row(Op.PLUS, XYPosition);

    Mat baseJointToEndEffectorJointXYPosition = rotatedAndShiftedEndEffectorJointXYPosition.minus(_prismaticJointStartXYPosition.plus(_prismaticJointXYDistance.elemTimes(prismaticJointRelativeDistances)));

    // See the paper cited in the class description for more information about the exact position of the angle (called
    // psi within the paper).
    Mat passiveJointAngles = Arma.acos((Arma.sumMat(Arma.square(baseJointToEndEffectorJointXYPosition), 1).minus(Arma.sumMat(Arma.square(_linkLengths), 1))).elemDivide(Arma.prodMat(_linkLengths, 1).times(2)));

    // See the paper cited in the class description for more information about the exact position of the angle (called
    // theta within the paper).
    Mat activeJointAngles = Trigonometry.atan2(
      (_linkLengths.col(0).plus(_linkLengths.col(1).elemTimes(Arma.cos(passiveJointAngles))).elemTimes(baseJointToEndEffectorJointXYPosition.col(1))).minus(_linkLengths.col(1).elemTimes(Arma.sin(passiveJointAngles)).elemTimes(baseJointToEndEffectorJointXYPosition.col(0))),
      (_linkLengths.col(0).plus(_linkLengths.col(1).elemTimes(Arma.cos(passiveJointAngles))).elemTimes(baseJointToEndEffectorJointXYPosition.col(0))).plus(_linkLengths.col(1).elemTimes(Arma.sin(passiveJointAngles)).elemTimes(baseJointToEndEffectorJointXYPosition.col(1)))
      );

    _directJacobian.col(0, Op.EQUAL, baseJointToEndEffectorJointXYPosition.col(0).minus(_linkLengths.col(0).elemTimes(Arma.cos(activeJointAngles))));
    _directJacobian.col(1, Op.EQUAL, baseJointToEndEffectorJointXYPosition.col(1).minus(_linkLengths.col(0).elemTimes(Arma.sin(activeJointAngles))));
    _directJacobian.col(2, Op.EQUAL, (_directJacobian.col(1).elemTimes(rotatedEndEffectorJointXYPosition.col(0))).minus(_directJacobian.col(0).elemTimes(rotatedEndEffectorJointXYPosition.col(1))));

    _inverseJacobian.diag(Op.EQUAL, _linkLengths.col(0).elemTimes((_directJacobian.col(0).elemTimes(Arma.sin(activeJointAngles))).minus(_directJacobian.col(1).elemTimes(Arma.cos(activeJointAngles)))));

    Arma.solve(_jacobian, _directJacobian, _inverseJacobian);
    return _jacobian;
  }
}
