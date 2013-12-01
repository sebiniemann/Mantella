package optimisation.util;

import arma.Mat;

/**
 * Provides sparse matrix helper functions.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
public class Sparse {
  
  /**
   * Returns the number of nonzero elements within a matrix.
   * 
   * @param matrix The matrix
   * @return The number of nonzero elements
   */
  public static int nnz(Mat matrix) {
    int numberOfNonzeroElements = 0;
    for (int n = 0; n < matrix.n_elem; n++) {
      if (matrix.at(n) != 0) {
        numberOfNonzeroElements++;
      }
    }

    return numberOfNonzeroElements;
  }
}
