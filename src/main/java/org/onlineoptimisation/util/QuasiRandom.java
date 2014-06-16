package org.onlineoptimisation.util;

import org.armadillojava.Mat;
import org.armadillojava.Op;

public class QuasiRandom {

  public static Mat getHaltonSequence(Mat seed, Mat base, int numberOfColumms) {
    Mat sequence = new Mat(seed.n_elem, numberOfColumms);

    for (int i = 0; i < seed.n_elem; i++) {
      sequence.row(i, Op.EQUAL, VanDerCorputSequence((int) seed.at(i), (int) base.at(i), numberOfColumms));
    }

    return sequence;
  }

  public static Mat VanDerCorputSequence(int seed, int base, int numberOfColumms) {
    Mat sequence = new Mat(1, numberOfColumms);

    for (int i = 0; i < numberOfColumms; i++) {
      double inverse = 1.0 / base;
      double number = 0;

      int remaining = seed + i;
      while (remaining > 0) {
        double digit = remaining % base;
        number = number + digit * inverse;
        inverse = inverse / base;
        remaining = (int) Math.floor(remaining / base);
      }

      sequence.at(i, Op.EQUAL, number);
    }

    return sequence;
  }
}
