package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Col;
import org.onlineoptimisation.problem.OptimisationProblem;

public class EllipsoidalFunction extends BenchmarkProblem {

  public EllipsoidalFunction(int numberOfDimensions) {
    super(numberOfDimensions);
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    double result = 0;
    
    for(int n = 0; n < parameter.n_elem; n++) {
      result += Math.pow(10, 6 * n / (_numberOfDimensions - 1)) * Math.pow(parameter.at(n), 2);
    }
    
    return result;
  }

}
