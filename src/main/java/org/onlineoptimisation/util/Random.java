package org.onlineoptimisation.util;

import org.onlineoptimisation.problem.OptimisationProblem;
import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Mat;
import org.armadillojava.Op;

public class Random {
  private static java.util.Random _RNG = new java.util.Random();

  public static java.util.Random getRNG() {
    return _RNG;
  }

  /**
   * @see Francesco Mezzadri, "How to generate random matrices from the classical compact groups", Notices of ACM,
   *      Volume 54, Number 5, 2007
   */
  public static Mat getRandomRotationMatrix(int numberOfDimensions) {
    Mat Q = new Mat();
    Mat R = new Mat();

    Arma.qr(Q, R, Arma.randn(numberOfDimensions, numberOfDimensions));

    Mat P = new Mat(numberOfDimensions, numberOfDimensions);
    P.diag(Op.EQUAL, R.diag().elemDivide(Arma.abs(R.diag())));

    Mat rotationMatrix = Q.times(P);

    return rotationMatrix;
  }

  public static Mat getRandomPopulation(int numberOfDimensions, int populationSize, Col lowerBound, Col upperBound) {
    Mat population = Arma.randu(numberOfDimensions, populationSize);

    population.each_col(Op.ELEMTIMES, upperBound.minus(lowerBound));
    population.each_col(Op.PLUS, lowerBound);

    return population;
  }

  public static Mat getRandomDirection(int numberOfDimensions) {
    Mat parameter = Arma.randn(numberOfDimensions, 1);
    return parameter.divide(Arma.norm(parameter, 2));
  }

  public static Mat getRandomDirection(OptimisationProblem optimisationProblem) {
    return Random.getRandomDirection(optimisationProblem.getNumberOfDimensions());
  }
}
