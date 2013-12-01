/*******************************************************************************
 * Copyright (c) 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
 *
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/MIT
 *******************************************************************************/
package optimisation.util;

import arma.Mat;
import arma.Op;

/**
 * Provides trigonometry helper functions.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
public class Trigonometry {

  /**
   * Returns the atan2({@code y.at(n)}, {@code x.at(n)}) between the {@code n}th element of both vectors for all
   * elements.
   * 
   * @param y The first vector
   * @param x The second vector
   * @return The atan2
   */
  public static Mat atan2(Mat y, Mat x) {
    Mat result = new Mat();
    result.copy_size(y);

    for (int n = 0; n < y.n_elem; n++) {
      result.at(n, Op.EQUAL, Math.atan2(y.at(n), x.at(n)));
    }

    return result;
  }
}
