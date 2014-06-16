package org.onlineoptimisation.util;

import org.armadillojava.Arma;
import org.armadillojava.Mat;

public class Arithmetic {
  public static Mat modulo(Mat a, Mat b) {
    return a.minus(Arma.floor(a.elemDivide(b)).elemTimes(a));
  }
}
