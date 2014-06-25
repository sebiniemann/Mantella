package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Mat;

public class BentCigarFunction extends BenchmarkProblem {

  protected Mat _R;
  
  public BentCigarFunction(int numberOfDimensions) {
    super(numberOfDimensions);
    
    _R = randomRotation();
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    Col z = Arma.square(_R.times(asymmetric(0.5, _R.times(parameter))));
    return  1000000 * Arma.accu(z) - 999999 * z.at(1);
  }

}
