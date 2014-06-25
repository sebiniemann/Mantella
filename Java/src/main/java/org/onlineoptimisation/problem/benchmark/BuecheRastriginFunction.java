package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Datum;
import org.armadillojava.Op;

public class BuecheRastriginFunction extends BenchmarkProblem {

  public BuecheRastriginFunction(int numberOfDimensions) {
    super(numberOfDimensions);
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    Col z = oscillate(parameter);
    
    Col scaling = scaling(Math.pow(10, 0.5));
    for(int n = 0; n < scaling.n_elem; n += 2) {
      if(z.at(n) > 0) {
        scaling.at(n, Op.TIMES, 10);
      }
    }
    z = z.elemTimes(scaling);
    
    return 10 * (_numberOfDimensions - Arma.accu(Arma.cos(z.times(2 * Datum.pi)))) + Arma.accu(Arma.square(z));
  }
  
  @Override
  public double getSoftConstraintValueImplementation(Col parameter) {
    return 100 * penality(parameter);
  }

}
