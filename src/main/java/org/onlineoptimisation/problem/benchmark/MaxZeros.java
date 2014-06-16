package org.onlineoptimisation.problem.benchmark;

import org.onlineoptimisation.problem.OptimisationProblem;

import org.armadillojava.Arma;
import org.armadillojava.Col;

public class MaxZeros extends BenchmarkProblem {

  public MaxZeros(int numberOfDimensions) {
    super(numberOfDimensions);
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    return Arma.accu(parameter);
  }
}
