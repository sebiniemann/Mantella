package org.onlineoptimisation.problem.robotic;

import org.armadillojava.Arma;
import org.armadillojava.Mat;
import org.armadillojava.Op;

public class Robot_6PUPS extends Robot {

  @Override
  public Mat getDirectJacobian(Mat parameter, Mat trajectoryPoint, boolean homogeneous) {

    double endEffectorAngleAxisX = trajectoryPoint.at(4);
    double endEffectorAngleAxisY = trajectoryPoint.at(5);
    double endEffectorAngleAxisZ = trajectoryPoint.at(6);

    Mat endEffectorRotationMatrixAxisX = new Mat(new double[][]{{
      1, 0, 0
    }, {
      0, Math.cos(endEffectorAngleAxisX), -Math.sin(endEffectorAngleAxisX)
    }, {
      0, Math.sin(endEffectorAngleAxisX), Math.cos(endEffectorAngleAxisX)
    }});

    Mat endEffectorRotationMatrixAxisY = new Mat(new double[][]{{
      Math.cos(endEffectorAngleAxisY), 0, Math.sin(endEffectorAngleAxisY)
    }, {
      0, 1, 0
    }, {
      -Math.sin(endEffectorAngleAxisY), 0, Math.cos(endEffectorAngleAxisY)
    }});

    Mat endEffectorRotationMatrixAxisZ = new Mat(new double[][]{{
      Math.cos(endEffectorAngleAxisZ), -Math.sin(endEffectorAngleAxisZ), 0
    }, {
      Math.sin(endEffectorAngleAxisZ), Math.cos(endEffectorAngleAxisZ), 0
    }, {
      0, 0, 1
    }});

    Mat endEffectorRotationMatrix = endEffectorRotationMatrixAxisX.times(endEffectorRotationMatrixAxisY).times(endEffectorRotationMatrixAxisZ);
    
    Mat endEffectorSphericalJointRelativePosition = new Mat(new double[][]{{
      -0.304055915910215, 0.304055915910215, 0
    }, {
      0.304055915910215, 0.304055915910215, 0
    }, {
      0.415348105304299, 0.111292189394084, 0
    }, {
      0.111292189394084,  -0.415348105304299, 0
    }, {
      -0.111292189394084, -0.415348105304299, 0
    }, {
      -0.415348105304299, 0.111292189394084, 0
    }});
    
    Mat activeUniversalJointPosition = new Mat(new double[][]{{
      -0.023293714059227, 0.086933324366016, 0
    }, {
      0.023293714059227, 0.086933324366016, 0
    }, {
      0.086933324366016, -0.023293714059227, 0
    }, {
      0.063639610306789,  -0.063639610306789, 0
    }, {
      -0.063639610306789, -0.063639610306789, 0
    }, {
      -0.086933324366016, -0.023293714059227, 0
    }});
    
    Mat endEffectorSphericalJointRotatedPosition = endEffectorRotationMatrix.times(endEffectorSphericalJointRelativePosition);
    
    Mat endEffectorSphericalJointPosition = endEffectorSphericalJointRotatedPosition.plus(trajectoryPoint);
    
    activeUniversalJointPosition.col(2, Op.PLUS, parameter);
    
    Mat prismaticJointRelativePosition = endEffectorSphericalJointRelativePosition.minus(endEffectorSphericalJointRelativePosition);
    
    return Arma.join_horiz(prismaticJointRelativePosition, Arma.cross(endEffectorSphericalJointPosition.t(), prismaticJointRelativePosition));
  }

}
