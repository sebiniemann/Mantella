package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Datum;

public class RastriginFunction extends BenchmarkProblem {

  public RastriginFunction(int numberOfDimensions) {
    super(numberOfDimensions);
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    Col z = delta(10).times(asymmetric(0.2, oscillate(parameter)));
    return 10 * (_numberOfDimensions - Arma.accu(Arma.cos(z.times(2 * Datum.pi)))) + Arma.accu(Arma.square(z));
  }

}
