package org.onlineoptimisation.problem.robotic.redundancySolution;

import org.onlineoptimisation.problem.OptimisationProblem;
import org.onlineoptimisation.problem.robotic.Robot;
import org.armadillojava.Mat;

public abstract class RedundancySolution extends OptimisationProblem {

  Robot _robot;
  Mat _trajectory;
  
  public RedundancySolution(Robot robot, int numberOfDimensions) {
    super(numberOfDimensions);
    _robot = robot;
  }

  protected Robot getRobot() {
    return _robot;
  }
  
  public final void setTrajectory(Mat trajectory) {
   _trajectory = trajectory; 
  }
  
  public Mat getTrajectory() {
    return _trajectory;
  }
  
  public final double getObjectiveValueImplementation(Mat parameter) {
    double fitness = -1;
//    for (int i = 0; i < _trajectory.n_cols; i++) {
//      double fitnessCandidate = getObjectiveValueImplementation(parameter, _trajectory.col(i));
//      fitness = Math.max(fitnessCandidate, fitness);
//    }

    return fitness;
  }
  
  protected abstract double getObjectiveValueImplementation(Mat parameter, Mat trajectory);
}
