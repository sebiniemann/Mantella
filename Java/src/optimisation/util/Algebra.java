package optimisation.util;

import arma.Arma;
import arma.Mat;
import arma.Op;

/**
 * Provides trigonometry helper functions.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
public class Algebra {

  /**
   * Returns the cross of all elements for each column of matrix1 and matrix2.
   * 
   * @param matrix1 The first matrix
   * @param matrix2 The second matrix
   * @return The matrix
   */
  public static Mat crossMat(Mat matrix1, Mat matrix2) {
    return crossMat(matrix1, matrix2, 0);
  }

  /**
   * Returns the cross of all elements for each column ({@code dimension} = 0) or row ({@code dimension} = 1) of matrix1 and matrix2.
   * 
   * @param matrix1 The first matrix
   * @param matrix2 The second matrix
   * @param dimension The dimension
   * @return The matrix
   */
  public static Mat crossMat(Mat matrix1, Mat matrix2, int dimension) {
    Mat result;
    if (dimension == 0) {
      result = new Mat(3, matrix1.n_cols);
      for (int j = 0; j < matrix1.n_cols; j++) {
        result.col(j, Op.EQUAL, Arma.cross(matrix1.col(j), matrix2.col(j)));
      }
    } else {
      result = new Mat(matrix1.n_rows, 3);
      for (int i = 0; i < matrix1.n_rows; i++) {
        result.row(i, Op.EQUAL, Arma.cross(matrix1.row(i), matrix2.row(i)));
      }
    }

    return result;
  }
  
  /**
   * Returns the {@code p}-norm of all elements for each column of the matrix.
   * 
   * @param matrix The matrix
   * @param p The type
   * @return The norm
   */
  public static Mat normMat(Mat matrix, int p) {
    return normMat(matrix, p, 0);
  }

  /**
   * Returns the {@code p}-norm of all elements for each column of the matrix.
   * 
   * @param matrix The matrix
   * @param p The type
   * @return The norm
   */
  public static Mat normMat(Mat matrix, String p) {
    return normMat(matrix, p, 0);
  }
  
  /**
   * Computes the {@code p}-norm of all elements for each column ({@code dimension} = 0) or row ({@code dimension} = 1)
   * matrix.
   * 
   * @param matrix The matrix
   * @param p The type
   * @param dimension The dimension
   * @return The norm
   */
  public static Mat normMat(Mat matrix, int p, int dimension) {
    Mat result;
    if (dimension == 0) {
      result = new Mat(1, matrix.n_cols);
      for (int j = 0; j < matrix.n_cols; j++) {
        result.at(j, Op.EQUAL, Arma.norm(matrix.col(j), p));
      }
    } else {
      result = new Mat(matrix.n_rows, 1);
      for (int i = 0; i < matrix.n_rows; i++) {
        result.at(i, Op.EQUAL, Arma.norm(matrix.row(i), p));
      }
    }

    return result;
  }

  /**
   * Computes the {@code p}-norm of all elements for each column ({@code dimension} = 0) or row ({@code dimension} = 1)
   * matrix.
   * 
   * @param matrix The matrix
   * @param p The type
   * @param dimension The dimension
   * @return The norm
   */
  public static Mat normMat(Mat matrix, String p, int dimension) {
    Mat result;
    if (dimension == 0) {
      result = new Mat(1, matrix.n_cols);
      for (int j = 0; j < matrix.n_cols; j++) {
        result.at(j, Op.EQUAL, Arma.norm(matrix.col(j), p));
      }
    } else {
      result = new Mat(matrix.n_rows, 1);
      for (int i = 0; i < matrix.n_rows; i++) {
        result.at(i, Op.EQUAL, Arma.norm(matrix.row(i), p));
      }
    }

    return result;
  }
}
