/*******************************************************************************
 * Copyright 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
 * 
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://opensource.org/licenses/MIT
 *******************************************************************************/
package optimisation;

import static org.junit.Assert.assertEquals;
import arma.Mat;

/**
 * Provides methods for often repeated tasks within test cases.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
public class TestUtil {

  /**
   * Asserts that the provides Matrices are element-wise equal to within a positive delta and have the same layout.
   * <p>
   * The assertion is based on org.junit.Assert.assertEquals.
   * 
   * @param message The message
   * @param A The expected matrix
   * @param B The actual matrix
   */
  public static void assertMatElementWiseEquals(String message, Mat A, Mat B) {
    assertMatElementWiseEquals(message, A, B, 1e-11);
  }

  /**
   * Asserts that the provides Matrices are element-wise equal to within a positive delta and have the same layout.
   * <p>
   * The assertion is based on org.junit.Assert.assertEquals.
   * 
   * @param message The message
   * @param A The expected matrix
   * @param B The actual matrix
   * @param delta The maximum allowed element-wise delta
   */
  public static void assertMatElementWiseEquals(String message, Mat A, Mat B, double delta) {
    assertEquals(message + ". Number of rows", A.n_rows, B.n_rows);
    assertEquals(message + ". Number of columns", A.n_cols, B.n_cols);

    for (int n = 0; n < A.n_elem; n++) {
      double a = A.at(n);
      double b = B.at(n);

      if (Double.isInfinite(a)) {
        assertEquals(message + " at position " + n, a, b, 0);
      } else {
        assertEquals(message + " at position " + n, a, b, ((a == 0) ? 1 : Math.abs(a)) * delta);
      }
    }
  }

  /**
   * Asserts that the provides matrices are equal to within a positive delta and have the same layout.
   * <p>
   * The assertion is based on org.junit.Assert.assertEquals.
   * 
   * @param message The message
   * @param A The expected matrix
   * @param B The actual matrix
   */
  public static void assertMatEquals(String message, Mat A, Mat B) {
    assertMatEquals(message, A, B, 1e-11);
  }

  /**
   * Asserts that the provides matrices are equal to within a positive delta and have the same layout.
   * <p>
   * The assertion is based on org.junit.Assert.assertEquals.
   * 
   * @param message The message
   * @param A The expected matrix
   * @param B The actual matrix
   * @param delta The maximum allowed delta
   */
  public static void assertMatEquals(String message, Mat A, Mat B, double delta) {
    assertEquals(message + ". Number of rows", A.n_rows, B.n_rows);
    assertEquals(message + ". Number of columns", A.n_cols, B.n_cols);

    double absoluteMax = 0;
    for (int n = 0; n < A.n_elem; n++) {
      double a = Math.abs(A.at(n));

      if (absoluteMax < a) {
        absoluteMax = a;
      }
    }

    if (absoluteMax == 0) {
      absoluteMax = 1;
    }

    for (int n = 0; n < A.n_elem; n++) {
      double a = A.at(n);
      double b = B.at(n);

      if (Double.isInfinite(a)) {
        assertEquals(message + " at position " + n, a, b, 0);
      } else {
        assertEquals(message + " at position " + n, a, b, absoluteMax * delta);
      }
    }
  }
}
