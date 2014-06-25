package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Mat;
import org.armadillojava.Op;

public class AttractiveSectorFunction extends BenchmarkProblem {

  protected Mat _Q;
  protected Mat _R;
  
  public AttractiveSectorFunction(int numberOfDimensions) {
    super(numberOfDimensions);
    
    _Q = randomRotation();
    _R = randomRotation();
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    Col z = _Q.times(delta(10).times(_R.times(parameter)));
    z.elem(Arma.find((z.elemTimes(_parameterShift)).strictGreaterThan(0)), Op.TIMES, 100);

    return oscillate(Math.pow(Arma.accu(Arma.square(z)), 0.9));
  }

}
