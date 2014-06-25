package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Op;

public class LinearSlope extends BenchmarkProblem {

  public LinearSlope(int numberOfDimensions) {
    super(numberOfDimensions);

    if(Arma.as_scalar(Arma.randu(1, 1)) < 0.5) {
      setParameterShift(Arma.zeros(Col.class, _numberOfDimensions).plus(5));
    } else {
      setParameterShift(Arma.zeros(Col.class, _numberOfDimensions).minus(5));
    }
  }

  @Override
  public double getObjectiveValueImplementation(Col parameter) {
    Col z = new Col(parameter);
    Col outOfBound = Arma.find((z.elemTimes(_parameterShift)).strictGreaterThan(25));
    z.elem(outOfBound, Op.EQUAL, _parameterShift.elem(outOfBound));

    Col scaling = scaling(1).elemTimes(Arma.sign(_parameterShift));
    return 5 * Arma.accu(Arma.abs(scaling)) - Arma.dot(scaling, z);
  }

}
