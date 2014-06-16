package org.onlineoptimisation.problem.robotic;

import org.armadillojava.Mat;

public class Robot_3PRRR extends Robot {

  Mat _redundantJointStartPosition;
  Mat _redundantJointDistance;
  Mat _endEffectorJointRelativePosition;
  Mat _linkLength;

  public Robot_3PRRR() {
    _redundantJointStartPosition = new Mat(new double[][]{
      {0.090782289181018, 0.0, 1.1996}, // X-Axis
      {1.001794428720682, -0.2, -0.2} // Y-Axis
    });

    _redundantJointDistance = new Mat(new double[][]{
      {0.999975421637964, 0, 0}, // X-Axis
      {0.007011142558636, 1, 1} // Y-Axis
    });

    _endEffectorJointRelativePosition = new Mat(new double[][]{
      {-0.000066580445834, -0.092751709777083, 0.092818290222917}, // X-Axis
      {0.106954081945581, -0.053477040972790, -0.053477040972790} // Y-Axis
    });

    _linkLength = new Mat(new double[][]{
      {0.58215, 0.58152, 0.58121}, // First linkage (near the end effector)
      {0.61975, 0.62002, 0.61964} // Second linkage
    });
  }

  @Override
  public Mat getDirectJacobian(Mat parameter, Mat trajectoryPoint, boolean homogeneous) {
//    Mat directJacobian = new Mat(3, 3);
//
//    Mat prismaticJointDistance = new Mat(_redundantJointDistance);
//    prismaticJointDistance.each_row(Op.ELEMTIMES, parameter);
//    
//    Mat baseJointPosition = _redundantJointStartPosition.plus(prismaticJointDistance);
//    
//    Mat endEffectorJointPosition = _endEffectorJointRelativePosition.plus(trajectoryPoint);
//
//    Mat baseJointToEndEffectorJointDistance = endEffectorJointPosition.minus(baseJointPosition);
//    
//    Mat passiveJointAngles = Arma.acos(Arma.square(baseJointToEndEffectorJointDistance.col(0)).plus(Arma.square(baseJointToEndEffectorJointDistance.col(1))).minus(Arma.square(_linkLength.row(0))).minus(Arma.square(_linkLength.row(1))).elemDivide(_linkLength.row(0).elemTimes(_linkLength.row(1).times(2))));
//      
//    Mat activeJointAngles = Trigonometry.atan2(
//      (_linkLength.row(0).plus(_linkLength.row(1).elemTimes(Arma.cos(passiveJointAngles))).elemTimes(baseJointToEndEffectorJointDistance.col(1)).minus(_linkLength.row(1).elemTimes(Arma.sin(passiveJointAngles)).elemTimes(baseJointToEndEffectorJointDistance.col(0)))),
//        (_linkLength.row(0).plus(_linkLength.row(1).elemTimes(Arma.cos(passiveJointAngles))).elemTimes(baseJointToEndEffectorJointDistance.col(1)).minus((_linkLength.row(1).elemTimes(Arma.sin(passiveJointAngles)).elemTimes(baseJointToEndEffectorJointDistance.col(0))
//          );
//
//    directJacobian.col(0, Op.EQUAL, endEffectorJointPosition.minus(baseJointPosition).minus(Arma.cos(activeJointAngles).elemTimes(_linkLength)));
//    directJacobian.col(1, Op.EQUAL, endEffectorJointPosition.minus(baseJointPosition).minus(Arma.sin(activeJointAngles).elemTimes(_linkLength)));
//    directJacobian.col(2, Op.EQUAL, directJacobian.col(1).elemTimes(endEffectorJointPosition.col(0).times(Math.cos(trajectoryPoint.at(3))).minus(endEffectorJointPosition.col(1).times(Math.sin(trajectoryPoint.at(3))))).minus(directJacobian.col(0).elemTimes(endEffectorJointPosition.col(0).times(Math.sin(trajectoryPoint.at(3))).plus(endEffectorJointPosition.col(1).times(Math.cos(trajectoryPoint.at(3)))))));
//
//    if (homogeneous) {
//      directJacobian.col(2, Op.ELEMDIVIDE, Arma.mean(_linkLength));
//    }

    return null;
  }
}
