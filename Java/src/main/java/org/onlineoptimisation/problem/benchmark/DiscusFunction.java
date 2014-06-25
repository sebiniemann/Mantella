package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Mat;

public class DiscusFunction extends BenchmarkProblem {

  protected Mat _R;
  
  public DiscusFunction(int numberOfDimensions) {
    super(numberOfDimensions);
    
    _R = randomRotation();
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    Col z = Arma.square(oscillate(_R.times(parameter)));
    return 999999 * z.at(1) + Arma.accu(z);
  }

}
