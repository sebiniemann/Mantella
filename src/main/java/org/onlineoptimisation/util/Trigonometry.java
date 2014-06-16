package org.onlineoptimisation.util;

import org.armadillojava.Arma;
import org.armadillojava.Mat;

public class Trigonometry {
  public static Mat atan2(Mat a, Mat b) {
    return Arma.atan((Arma.sqrt(Arma.square(a).plus(Arma.square(b))).minus(a)).elemDivide(b));
  }
}
