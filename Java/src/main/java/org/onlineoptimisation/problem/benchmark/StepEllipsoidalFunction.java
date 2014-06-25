package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Mat;

public class StepEllipsoidalFunction extends BenchmarkProblem {

  protected Mat _Q;
  protected Mat _R;
  
  public StepEllipsoidalFunction(int numberOfDimensions) {
    super(numberOfDimensions);
    
    _Q = randomRotation();
    _R = randomRotation();
  }

  @Override
  protected double getObjectiveValueImplementation(Col parameter) {
    return Arma.dot(scaling(1000000), Arma.square(oscillate(parameter)));
  }

}
