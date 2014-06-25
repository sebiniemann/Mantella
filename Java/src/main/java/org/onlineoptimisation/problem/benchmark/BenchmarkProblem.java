package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Fill;
import org.armadillojava.Mat;
import org.armadillojava.Op;

import org.onlineoptimisation.problem.OptimisationProblem;

public abstract class BenchmarkProblem extends OptimisationProblem {

  public BenchmarkProblem(int numberOfDimensions) {
    super(numberOfDimensions);

    setParameterShift(Arma.randu(Col.class, _numberOfDimensions).times(8).minus(4));
    setLowerBounds(Arma.zeros(Col.class, _numberOfDimensions).minus(5));
    setUpperBounds(Arma.zeros(Col.class, _numberOfDimensions).plus(5));
    setObjectiveValueShift(Math.min(1000, Math.max(-1000, (Math.round(100 * 100 * Arma.as_scalar(Arma.randn(1, 1)) / Arma.as_scalar(Arma.randn(1, 1))) / 100))));
  }

  protected Mat delta(double alpha) {
    Mat delta = new Mat(_numberOfDimensions, _numberOfDimensions, Fill.EYE);

    for (int i = 0; i < delta.n_rows; i++) {
      delta.at(i, i, Op.EQUAL, Math.pow(alpha, 0.5 * (double) (i / (_numberOfDimensions - 1))));
    }

    return delta;
  }

  protected double penality(Col parameter) {
    double max = 0;
    
    for (int n = 0; n < parameter.n_elem; n++) {
      max += Math.pow(Math.max(0, Math.abs(parameter.at(n)) - 5), 2);
    }
    
    return max;
  }

  protected Col asymmetric(double beta, Col parameter) {
    Col asy = new Col(parameter);

    for (int n = 0; n < asy.n_elem; n++) {
      double value = asy.at(n);

      if (value > 0) {
        asy.at(n, Op.EQUAL, Math.pow(value, 1 + beta * (double) (n / (_numberOfDimensions - 1)) * Math.sqrt(value)));
      }
    }

    return asy;
  }

  protected double oscillate(double parameter) {
    double x;
    if (parameter != 0) {
      x = Math.log(Math.abs(parameter));
    } else {
      x = 0;
    }

    double c1;
    double c2;
    if (parameter > 0) {
      c1 = 10;
      c2 = 7.9;
    } else {
      c1 = 5.5;
      c2 = 3.1;
    }

    return Math.signum(parameter) * Math.exp(x + 0.049 * (Math.sin(c1 * x) + Math.sin(c2 * x)));
  }

  protected Col oscillate(Col parameter) {
    Col oscillate = new Col(parameter);

    for (int n = 0; n < oscillate.n_elem; n++) {
      oscillate.at(n, Op.EQUAL, oscillate(oscillate.at(n)));
    }

    return oscillate;
  }

  public Mat randomRotation() {
    Mat rotationMatrix = Arma.randn(_numberOfDimensions, _numberOfDimensions);
    for(int n = 0; n < rotationMatrix.n_rows; n++) {
      for(int m = 0; m < n - 1; m++) {
        rotationMatrix.col(n, Op.MINUS, rotationMatrix.col(m).times(Arma.dot(rotationMatrix.col(n), rotationMatrix.col(m))));
      }
      rotationMatrix.col(n, Op.DIVIDE, Math.sqrt(Arma.accu(Arma.square(rotationMatrix.col(n)))));
    }

    return rotationMatrix;
  }

  public Col scaling(double conditioning) {
    Col scaling = Arma.linspace(Col.class, 0, 1, _numberOfDimensions);

    for (int n = 0; n < scaling.n_elem; n++) {
      scaling.at(n, Op.EQUAL, Math.pow(conditioning, scaling.at(n)));
    }

    return scaling;
  }

}
