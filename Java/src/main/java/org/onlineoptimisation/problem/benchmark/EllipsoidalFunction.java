package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;

public class EllipsoidalFunction extends BenchmarkProblem {

  public EllipsoidalFunction(int numberOfDimensions) {
    super(numberOfDimensions);
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    return Arma.dot(scaling(1000000), Arma.square(oscillate(parameter)));
  }

}
