package org.onlineoptimisation.problem.robotic;

import org.armadillojava.Arma;
import org.armadillojava.Datum;
import org.armadillojava.Fill;
import org.armadillojava.Mat;

public class Robot_3RUS {

  Mat _baseJointPosition;
  Mat _endEffectorJointRelativePosition;
  Mat _linkLength;

  Mat _baseJoint1RotationMatrixAxisY;
  Mat _baseJoint2RotationMatrixAxisY;
  Mat _baseJoint3RotationMatrixAxisY;

  public Robot_3RUS() {
    Mat baseJointAngleAxisY = new Mat(new double[][]{
      {0},
      {2 * Datum.pi / 3},
      {4 * Datum.pi / 3}
    });

    _baseJointPosition = new Mat(new double[][]{
      {Math.cos(baseJointAngleAxisY.at(0)), Math.sin(baseJointAngleAxisY.at(0)), 0}, // X-Axis
      {Math.cos(baseJointAngleAxisY.at(1)), Math.sin(baseJointAngleAxisY.at(1)), 0}, // Y-Axis
      {Math.cos(baseJointAngleAxisY.at(2)), Math.sin(baseJointAngleAxisY.at(2)), 0} // Z-Axis
    }).times(0.1);

    _endEffectorJointRelativePosition = new Mat(new double[][]{
      {Math.cos(baseJointAngleAxisY.at(0)), Math.sin(baseJointAngleAxisY.at(0)), 0}, // X-Axis
      {Math.cos(baseJointAngleAxisY.at(1)), Math.sin(baseJointAngleAxisY.at(1)), 0}, // Y-Axis
      {Math.cos(baseJointAngleAxisY.at(2)), Math.sin(baseJointAngleAxisY.at(2)), 0} // Z-Axis
    }).times(0.0225);

    // This should be optimised
    _linkLength = new Mat(new double[][]{
      {0.8, 0.8, 0.8}, // First linkage (near the end effector)
      {0.35, 0.35, 0.35} // Second linkage
    });

    _baseJoint1RotationMatrixAxisY = new Mat(new double[][]{
      {Math.cos(baseJointAngleAxisY.at(0)), 0, Math.sin(baseJointAngleAxisY.at(0))},
      {0, 1, 0},
      {-Math.sin(baseJointAngleAxisY.at(0)), 0, Math.cos(baseJointAngleAxisY.at(0))}
    });

    _baseJoint2RotationMatrixAxisY = new Mat(new double[][]{
      {Math.cos(baseJointAngleAxisY.at(1)), 0, Math.sin(baseJointAngleAxisY.at(1))},
      {0, 1, 0},
      {-Math.sin(baseJointAngleAxisY.at(1)), 0, Math.cos(baseJointAngleAxisY.at(1))}
    });

    _baseJoint3RotationMatrixAxisY = new Mat(new double[][]{
      {Math.cos(baseJointAngleAxisY.at(2)), 0, Math.sin(baseJointAngleAxisY.at(2))},
      {0, 1, 0},
      {-Math.sin(baseJointAngleAxisY.at(2)), 0, Math.cos(baseJointAngleAxisY.at(2))}
    });

  }

  /**
   * Calculates the Jacobian of the non-redundant 3RUS mechanism.
   * 
   * @see Kotlarski, J.; Heimann, B.; Ortmaier, T. (2012): Influence of Kinematic Redundancy on the Singularity-free
   *      Workspace of Parallel Kinematic Machines, Frontiers of Mechanical Engineering, Vol. 7(2), pp. 120-134
   * 
   * @return The Jacobian
   */
  public Mat getJacobian(Mat pose) {
    return Arma.solve(getDirectJacobian(pose), getInverseJacobian(pose));
  }

  /**
   * Calculates the direct Jacobian of the non-redundant 3RUS mechanism.
   * 
   * @see Kotlarski, J.; Heimann, B.; Ortmaier, T. (2012): Influence of Kinematic Redundancy on the Singularity-free
   *      Workspace of Parallel Kinematic Machines, Frontiers of Mechanical Engineering, Vol. 7(2), pp. 120-134
   * 
   * @return The direct Jacobian
   */
  public Mat getDirectJacobian(Mat pose) {
    Mat directJacobian = new Mat(3, 6);

//    Mat position = pose.subvec(0, 2);
//    Mat orientation = pose.subvec(3, 5);
//
//    Mat endEffectorTaitBryanZYXRotationMatrix = new Mat(new double[][]{
//      {Math.cos(orientation.at(0)) * Math.cos(orientation.at(1)), Math.cos(orientation.at(0)) * Math.sin(orientation.at(1)) * Math.sin(orientation.at(2)) - Math.cos(orientation.at(2)) * Math.sin(orientation.at(0)), Math.sin(orientation.at(0)) * Math.sin(orientation.at(2)) + Math.cos(orientation.at(0)) * Math.cos(orientation.at(2)) * Math.sin(orientation.at(1))},
//      {-Math.cos(orientation.at(1)) * Math.sin(orientation.at(0)), -Math.cos(orientation.at(0)) * Math.cos(orientation.at(2)) + Math.sin(orientation.at(0)) * Math.sin(orientation.at(1)) * Math.sin(orientation.at(2)), -Math.cos(orientation.at(2)) * Math.sin(orientation.at(0)) * Math.sin(orientation.at(1)) - Math.cos(orientation.at(0)) * Math.sin(orientation.at(2))},
//      {-Math.sin(orientation.at(1)), Math.cos(orientation.at(1)) * Math.sin(orientation.at(2)), Math.cos(orientation.at(1)) * Math.cos(orientation.at(2))}
//    });
//
//    Mat endEffectorJoint1PositionRelativeToBaseJoint1 = _baseJoint1RotationMatrixAxisY.times(position.plus(endEffectorTaitBryanZYXRotationMatrix.times(_endEffectorJointRelativePosition.col(0))).minus(_baseJointPosition.col(0)));
//    Mat endEffectorJoint2PositionRelativeToBaseJoint2 = _baseJoint2RotationMatrixAxisY.times(position.plus(endEffectorTaitBryanZYXRotationMatrix.times(_endEffectorJointRelativePosition.col(1))).minus(_baseJointPosition.col(1)));
//    Mat endEffectorJoint3PositionRelativeToBaseJoint3 = _baseJoint3RotationMatrixAxisY.times(position.plus(endEffectorTaitBryanZYXRotationMatrix.times(_endEffectorJointRelativePosition.col(2))).minus(_baseJointPosition.col(2)));
//
//    double baseJointAngle1 = Math.atan2(endEffectorJoint1PositionRelativeToBaseJoint1.at(1), endEffectorJoint1PositionRelativeToBaseJoint1.at(0)) + Math.acos((Arma.sum(Arma.square(endEffectorJoint1PositionRelativeToBaseJoint1)) + Math.pow(_linkLength.at(0, 0), 2) - Math.pow(_linkLength.at(1, 0), 2)) / (2 * _linkLength.at(0, 0) * Math.sqrt(Math.pow(endEffectorJoint1PositionRelativeToBaseJoint1.at(0), 2) + Math.pow(endEffectorJoint1PositionRelativeToBaseJoint1.at(1), 2))));
//    double baseJointAngle2 = Math.atan2(endEffectorJoint2PositionRelativeToBaseJoint2.at(1), endEffectorJoint2PositionRelativeToBaseJoint2.at(0)) + Math.acos((Arma.sum(Arma.square(endEffectorJoint2PositionRelativeToBaseJoint2)) + Math.pow(_linkLength.at(0, 1), 2) - Math.pow(_linkLength.at(1, 1), 2)) / (2 * _linkLength.at(0, 1) * Math.sqrt(Math.pow(endEffectorJoint2PositionRelativeToBaseJoint2.at(0), 2) + Math.pow(endEffectorJoint2PositionRelativeToBaseJoint2.at(1), 2))));
//    double baseJointAngle3 = Math.atan2(endEffectorJoint3PositionRelativeToBaseJoint3.at(1), endEffectorJoint3PositionRelativeToBaseJoint3.at(0)) + Math.acos((Arma.sum(Arma.square(endEffectorJoint3PositionRelativeToBaseJoint3)) + Math.pow(_linkLength.at(0, 2), 2) - Math.pow(_linkLength.at(1, 2), 2)) / (2 * _linkLength.at(0, 2) * Math.sqrt(Math.pow(endEffectorJoint3PositionRelativeToBaseJoint3.at(0), 2) + Math.pow(endEffectorJoint3PositionRelativeToBaseJoint3.at(1), 2))));
//
//    Mat passiveJointPosition = new Mat(3, 3);
//    
//    passiveJointPosition.col(0, Op.EQUAL, _baseJointPosition.col(0).plus(new Mat(new double[][]{
//      {Math.cos(orientation.at(0)) * Math.cos(baseJointAngle1)},
//      {Math.cos(orientation.at(0)) * Math.sin(baseJointAngle1)},
//      {Math.sin(baseJointAngle1)}
//    })).times(_linkLength.at(0, 0)));
//
//    passiveJointPosition.col(1, Op.EQUAL, _baseJointPosition.col(1).plus(new Mat(new double[][]{
//      {Math.cos(orientation.at(1)) * Math.cos(baseJointAngle2)},
//      {Math.cos(orientation.at(1)) * Math.sin(baseJointAngle2)},
//      {Math.sin(baseJointAngle2)}
//    })).times(_linkLength.at(0, 1)));
//
//    passiveJointPosition.col(2, Op.EQUAL, _baseJointPosition.col(2).plus(new Mat(new double[][]{
//      {Math.cos(orientation.at(2)) * Math.cos(baseJointAngle3)},
//      {Math.cos(orientation.at(2)) * Math.sin(baseJointAngle3)},
//      {Math.sin(baseJointAngle3)}
//    })).times(_linkLength.at(0, 2)));
//
//    Mat endEffectorJointRelativePosition = new Mat(_endEffectorJointRelativePosition);
//    endEffectorJointRelativePosition.each_col(Op.PLUS, position);
//
//    Mat passiveJointToEndEffectorJointPosition = endEffectorJointRelativePosition.minus(passiveJointPosition);
//    directJacobian.cols(0, 2, Op.EQUAL, passiveJointToEndEffectorJointPosition);
//    directJacobian.col(3, Op.EQUAL, endEffectorTaitBryanZYXRotationMatrix.times(_endEffectorJointRelativePosition.col(0)));
//    directJacobian.col(4, Op.EQUAL, endEffectorTaitBryanZYXRotationMatrix.times(_endEffectorJointRelativePosition.col(1)));
//    directJacobian.col(5, Op.EQUAL, endEffectorTaitBryanZYXRotationMatrix.times(_endEffectorJointRelativePosition.col(2)));

    return directJacobian;
  }

  /**
   * Calculates the inverse Jacobian of the non-redundant 3RUS mechanism.
   * 
   * @see Kotlarski, J.; Heimann, B.; Ortmaier, T. (2012): Influence of Kinematic Redundancy on the Singularity-free
   *      Workspace of Parallel Kinematic Machines, Frontiers of Mechanical Engineering, Vol. 7(2), pp. 120-134
   * 
   * @return The inverse Jacobian
   */
  public Mat getInverseJacobian(Mat pose) {
    Mat inverseJacobian = new Mat(3, 3, Fill.ZEROS);

//    Mat position = pose.subvec(0, 2);
//    Mat orientation = pose.subvec(3, 5);
//
//    Mat endEffectorTaitBryanZYXRotationMatrix = new Mat(new double[][]{
//      {Math.cos(orientation.at(0)) * Math.cos(orientation.at(1)), Math.cos(orientation.at(0)) * Math.sin(orientation.at(1)) * Math.sin(orientation.at(2)) - Math.cos(orientation.at(2)) * Math.sin(orientation.at(0)), Math.sin(orientation.at(0)) * Math.sin(orientation.at(2)) + Math.cos(orientation.at(0)) * Math.cos(orientation.at(2)) * Math.sin(orientation.at(1))},
//      {Math.cos(orientation.at(1)) * Math.sin(orientation.at(0)), Math.cos(orientation.at(0)) * Math.cos(orientation.at(2)) + Math.sin(orientation.at(0)) * Math.sin(orientation.at(1)) * Math.sin(orientation.at(2)), Math.cos(orientation.at(2)) * Math.sin(orientation.at(0)) * Math.sin(orientation.at(1)) - Math.cos(orientation.at(0)) * Math.sin(orientation.at(2))},
//      {-Math.sin(orientation.at(1)), Math.cos(orientation.at(1)) * Math.sin(orientation.at(2)), Math.cos(orientation.at(1)) * Math.cos(orientation.at(2))}
//    });
//
//    Mat endEffectorJoint1PositionRelativeToBaseJoint1 = _baseJoint1RotationMatrixAxisY.times(position.plus(endEffectorTaitBryanZYXRotationMatrix.times(_endEffectorJointRelativePosition.col(0))).minus(_baseJointPosition.col(0)));
//    Mat endEffectorJoint2PositionRelativeToBaseJoint2 = _baseJoint1RotationMatrixAxisY.times(position.plus(endEffectorTaitBryanZYXRotationMatrix.times(_endEffectorJointRelativePosition.col(1))).minus(_baseJointPosition.col(1)));
//    Mat endEffectorJoint3PositionRelativeToBaseJoint3 = _baseJoint1RotationMatrixAxisY.times(position.plus(endEffectorTaitBryanZYXRotationMatrix.times(_endEffectorJointRelativePosition.col(2))).minus(_baseJointPosition.col(2)));
//
//    double baseJointAngle1 = Math.atan2(endEffectorJoint1PositionRelativeToBaseJoint1.at(0), endEffectorJoint1PositionRelativeToBaseJoint1.at(1)) + Math.acos((Arma.sum(Arma.square(endEffectorJoint1PositionRelativeToBaseJoint1)) + Math.pow(_linkLength.at(0, 0), 2) - Math.pow(_linkLength.at(1, 0), 2)) / (2 * _linkLength.at(0, 0) * Math.sqrt(Math.pow(endEffectorJoint1PositionRelativeToBaseJoint1.at(0), 2) + Math.pow(endEffectorJoint1PositionRelativeToBaseJoint1.at(1), 2))));
//    double baseJointAngle2 = Math.atan2(endEffectorJoint2PositionRelativeToBaseJoint2.at(0), endEffectorJoint2PositionRelativeToBaseJoint2.at(1)) + Math.acos((Arma.sum(Arma.square(endEffectorJoint2PositionRelativeToBaseJoint2)) + Math.pow(_linkLength.at(0, 1), 2) - Math.pow(_linkLength.at(1, 1), 2)) / (2 * _linkLength.at(0, 1) * Math.sqrt(Math.pow(endEffectorJoint2PositionRelativeToBaseJoint2.at(0), 2) + Math.pow(endEffectorJoint2PositionRelativeToBaseJoint2.at(1), 2))));
//    double baseJointAngle3 = Math.atan2(endEffectorJoint3PositionRelativeToBaseJoint3.at(0), endEffectorJoint3PositionRelativeToBaseJoint3.at(1)) + Math.acos((Arma.sum(Arma.square(endEffectorJoint3PositionRelativeToBaseJoint3)) + Math.pow(_linkLength.at(0, 2), 2) - Math.pow(_linkLength.at(1, 2), 2)) / (2 * _linkLength.at(0, 2) * Math.sqrt(Math.pow(endEffectorJoint3PositionRelativeToBaseJoint3.at(0), 2) + Math.pow(endEffectorJoint3PositionRelativeToBaseJoint3.at(1), 2))));
//
//    inverseJacobian.at(0, 0, Op.EQUAL, _linkLength.at(0, 0) * (endEffectorJoint1PositionRelativeToBaseJoint1.at(0) * Math.sin(baseJointAngle1) - endEffectorJoint1PositionRelativeToBaseJoint1.at(1) * Math.cos(baseJointAngle1)));
//    inverseJacobian.at(1, 1, Op.EQUAL, _linkLength.at(0, 1) * (endEffectorJoint2PositionRelativeToBaseJoint2.at(0) * Math.sin(baseJointAngle2) - endEffectorJoint2PositionRelativeToBaseJoint2.at(1) * Math.cos(baseJointAngle2)));
//    inverseJacobian.at(2, 2, Op.EQUAL, _linkLength.at(0, 2) * (endEffectorJoint3PositionRelativeToBaseJoint3.at(0) * Math.sin(baseJointAngle3) - endEffectorJoint3PositionRelativeToBaseJoint3.at(1) * Math.cos(baseJointAngle3)));

    return inverseJacobian;

  }
}
