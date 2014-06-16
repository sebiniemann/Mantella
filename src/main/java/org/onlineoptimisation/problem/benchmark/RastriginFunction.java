package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Datum;
import org.onlineoptimisation.problem.OptimisationProblem;

public class RastriginFunction extends BenchmarkProblem {

  public RastriginFunction(int numberOfDimensions) {
    super(numberOfDimensions);
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    return 10 * (_numberOfDimensions - Arma.accu(Arma.cos(parameter.times(2 * Datum.pi)))) + Arma.accu(Arma.square(parameter));
  }

}
