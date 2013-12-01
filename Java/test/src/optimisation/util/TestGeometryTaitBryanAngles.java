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
public class TestGeometryTaitBryanAngles {
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
    for (double rollAngle : input) {
      for (double pitchAngle : input) {
        for (double yawAngle : input) {
          testMatrices.add(new Object[]{rollAngle, pitchAngle, yawAngle});
        }
      }
    }

    return testMatrices;
  }

  /**
   * The roll angle
   */
  @Parameter(0)
  public double _rollAngle;

  /**
   * The roll angle
   */
  @Parameter(1)
  public double _pitchAngle;

  /**
   * The roll angle
   */
  @Parameter(2)
  public double _yawAngle;

  /**
   * Test method for {@link optimisation.util.Geometry#getZYXTaitBryanAnglesRotationMatrix(double, double, double)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testGetZYXTaitBryanAnglesRotationMatrix() throws IOException {
    Mat expected = new Mat();

    String filename = TestUtil.convertToString(_rollAngle) + "." + TestUtil.convertToString(_pitchAngle) + "." + TestUtil.convertToString(_yawAngle) + ".mat";

    expected.load("./test/data/expected/util/TestGeometryTaitBryanAngles/testTaitBryanAngles." + filename);
    assertMatEquals(filename, expected, Geometry.getZYXTaitBryanAnglesRotationMatrix(_rollAngle, _pitchAngle, _yawAngle));
  }

}
