package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Mat;

public class EllipsoidalFunctionRotated extends BenchmarkProblem {

  protected Mat _R;
  
  public EllipsoidalFunctionRotated(int numberOfDimensions) {
    super(numberOfDimensions);
    
    _R = randomRotation();
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    return Arma.dot(scaling(1000000), Arma.square(oscillate(_R.times(parameter))));
  }

}
