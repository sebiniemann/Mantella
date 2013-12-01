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
public class TestUtilAlgebraNorm {
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

    int[] dimensions = {1, 2, 3, 4, 5, 10, 100};
    String matrices[] = {"zeros", "ones", "eye", "hankel", "hilbert"};

    for (int numberOfElements : dimensions) {
      Mat testMatrix = new Mat();
      testMatrix.set_size(numberOfElements, matrices.length);

      int j = 0;
      String filename = "1x" + numberOfElements + ".mat";
      for (String matrix : matrices) {
        if (matrix != "hilbert" || numberOfElements < 10) {
          Mat input = new Mat();
          input.load("./test/data/input/" + matrix + "." + filename);
          testMatrix.col(j++, Op.EQUAL, input);
        }
      }
      testMatrices.add(new Object[]{filename, testMatrix});
    }

    return testMatrices;
  }

  /**
   * The filename of the test matrix
   */
  @Parameter(0)
  public String _filename;

  /**
   * The test matrix
   */
  @Parameter(1)
  public Mat    _testMatrix;

  /**
   * Test method for {@link optimisation.util.Algebra#normMat(arma.Mat, int)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testNormMatMatInt() throws IOException {
    Mat expected = new Mat();

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.p1." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, 1));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.p2." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, 2));
  }

  /**
   * Test method for {@link optimisation.util.Algebra#normMat(arma.Mat, java.lang.String)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testNormMatMatString() throws IOException {
    Mat expected = new Mat();

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.pinf." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, "inf"));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.pfro." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, "fro"));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.p-inf." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, "-inf"));
  }

  /**
   * Test method for {@link optimisation.util.Algebra#normMat(arma.Mat, int, int)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testNormMatMatIntInt() throws IOException {
    Mat expected = new Mat();

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.p1." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, 1, 0));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.p2." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, 2, 0));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.p1." + _filename);
    assertMatElementWiseEquals("", expected.t(), Algebra.normMat(_testMatrix.t(), 1, 1));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.p2." + _filename);
    assertMatElementWiseEquals("", expected.t(), Algebra.normMat(_testMatrix.t(), 2, 1));
  }

  /**
   * Test method for {@link optimisation.util.Algebra#normMat(arma.Mat, java.lang.String, int)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testNormMatMatStringInt() throws IOException {
    Mat expected = new Mat();

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.pinf." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, "inf", 0));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.pfro." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, "fro", 0));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.p-inf." + _filename);
    assertMatElementWiseEquals("", expected, Algebra.normMat(_testMatrix, "-inf", 0));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.pinf." + _filename);
    assertMatElementWiseEquals("", expected.t(), Algebra.normMat(_testMatrix.t(), "inf", 1));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.pfro." + _filename);
    assertMatElementWiseEquals("", expected.t(), Algebra.normMat(_testMatrix.t(), "fro", 1));

    expected.load("./test/data/expected/TestUtilAlgebraNorm/testNorm.p-inf." + _filename);
    assertMatElementWiseEquals("", expected.t(), Algebra.normMat(_testMatrix.t(), "-inf", 1));
  }

}
