package org.onlineoptimisation.problem.robotic.redundancySolution;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Mat;
import org.onlineoptimisation.problem.robotic.Robot;

public class SpectralNormOfHomogeneousDirectJacobian extends RedundancySolution {

  public SpectralNormOfHomogeneousDirectJacobian(Robot robot, int numberOfDimensions) {
    super(robot, numberOfDimensions);
  }

  @Override
  public double getObjectiveValueImplementation(Mat parameter, Mat trajectory) {
    Col singularvalues = Arma.svd(getRobot().getDirectJacobian(parameter, trajectory, true));
    return -1 / (singularvalues.at(0) / singularvalues.at(2));
  }

  @Override
  protected double getObjectiveValueImplementation(Col parameter) {
    // TODO Auto-generated method stub
    return 0;
  }
}