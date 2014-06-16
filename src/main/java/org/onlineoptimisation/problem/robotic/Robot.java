package org.onlineoptimisation.problem.robotic;

import org.armadillojava.Mat;

public abstract class Robot {
  public abstract Mat getDirectJacobian(Mat parameter, Mat trajectoryPoint, boolean homogeneous);
}
