package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.onlineoptimisation.problem.OptimisationProblem;

public class SphereFunction extends OptimisationProblem {

  public SphereFunction(int numberOfDimensions) {
    super(numberOfDimensions);
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    return Arma.accu(Arma.square(parameter));
  }

}
