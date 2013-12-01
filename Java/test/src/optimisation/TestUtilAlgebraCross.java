/**
 * 
 */
package optimisation;

import static optimisation.TestUtil.assertMatElementWiseEquals;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;

import optimisation.util.Algebra;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameter;
import org.junit.runners.Parameterized.Parameters;

import arma.Mat;
import arma.Op;

/**
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
@RunWith(Parameterized.class)
public class TestUtilAlgebraCross {
  /**
   * 
   * Returns the matrices to be tested.
   * 
   * @return The test matrices
   * 
   * @throws IOException An I/O error occurred
   */
  @Parameters
  public static Collection<Object[]> getTestMatrices() throws IOException {
    Collection<Object[]> testMatrices = new ArrayList<Object[]>();

    String matrices[] = {"zeros", "ones", "eye", "hankel", "hilbert"};

    Mat inputA = new Mat();
    Mat inputB = new Mat();

    Mat testMatrix1 = new Mat();
    Mat testMatrix2 = new Mat();
    testMatrix1.set_size(3, (int) Math.pow(matrices.length, 2));
    testMatrix2.set_size(3, (int) Math.pow(matrices.length, 2));

    int j = 0;
    for (String matrixA : matrices) {
      for (String matrixB : matrices) {
        inputA.load("./test/data/input/" + matrixA + ".3x1.mat");
        inputB.load("./test/data/input/" + matrixB + ".3x1.mat");

        testMatrix1.col(j, Op.EQUAL, inputA);
        testMatrix2.col(j, Op.EQUAL, inputB);
        j++;
      }
    }

    testMatrices.add(new Object[]{testMatrix1, testMatrix2});

    return testMatrices;
  }

  /**
   * The first test matrix
   */
  @Parameter(0)
  public Mat _testMatrix1;

  /**
   * The second test matrix
   */
  @Parameter(1)
  public Mat _testMatrix2;

  /**
   * Test method for {@link optimisation.util.Algebra#crossMat(arma.Mat, arma.Mat)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testCrossMatMatMat() throws IOException {
    Mat expected = new Mat();
    expected.load("./test/data/expected/TestUtilAlgebraCross/testCross.mat");
    assertMatElementWiseEquals("", expected, Algebra.crossMat(_testMatrix1, _testMatrix2));
  }

  /**
   * Test method for {@link optimisation.util.Algebra#crossMat(arma.Mat, arma.Mat, int)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testCrossMatMatMatInt() throws IOException {
    Mat expected = new Mat();
    expected.load("./test/data/expected/TestUtilAlgebraCross/testCross.mat");
    assertMatElementWiseEquals("", expected, Algebra.crossMat(_testMatrix1, _testMatrix2, 0));
    assertMatElementWiseEquals("", expected.t(), Algebra.crossMat(_testMatrix1.t(), _testMatrix2.t(), 1));
  }
}
