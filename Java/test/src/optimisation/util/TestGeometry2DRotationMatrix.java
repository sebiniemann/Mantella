/**
 * 
 */
package optimisation.util;

import static optimisation.TestUtil.assertMatEquals;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;

import optimisation.TestUtil;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameter;
import org.junit.runners.Parameterized.Parameters;

import arma.Mat;

/**
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 * 
 */
@RunWith(Parameterized.class)
public class TestGeometry2DRotationMatrix {
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

    Mat input = new Mat();
    input.load("./test/data/input/rotation.mat");
    for (double angle : input) {
      testMatrices.add(new Object[]{angle});
    }

    return testMatrices;
  }

  /**
   * The angle
   */
  @Parameter
  public double _angle;

  /**
   * Test method for {@link optimisation.util.Geometry#get2DRotationMatrix(double)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testGet2DRotationMatrix() throws IOException {
    Mat expected = new Mat();

    String filename = TestUtil.convertToString(_angle) + ".mat";

    expected.load("./test/data/expected/util/TestGeometry2DRotationMatrix/testGeometry2DRotationMatrix." + filename);
    assertMatEquals(filename, expected, Geometry.get2DRotationMatrix(_angle));
  }

}
