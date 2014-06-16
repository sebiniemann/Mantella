package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.onlineoptimisation.problem.OptimisationProblem;

public class LinearSlope extends OptimisationProblem {

  protected final Col _optimalParameter;
  
  public LinearSlope(int numberOfDimensions) {
    super(numberOfDimensions);

    _optimalParameter = Arma.ones(Col.class, _numberOfDimensions).times(5);
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    double result = 0;
    
    for(int n = 0; n < parameter.n_elem; n++) {
      double s = Math.signum(_optimalParameter.at(n)) * Math.pow(10, n / (_numberOfDimensions - 1));
      result += 5 * Math.abs(s) - s * parameter.at(n);
    }
    
    return result;
  }

}
