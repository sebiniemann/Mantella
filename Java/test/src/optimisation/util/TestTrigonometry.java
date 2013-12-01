/**
 * 
 */
package optimisation.util;

import static optimisation.TestUtil.assertMatElementWiseEquals;

import java.io.IOException;

import org.junit.Test;

import arma.Mat;

/**
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 *
 */
public class TestTrigonometry {

  /**
   * Test method for {@link optimisation.util.Trigonometry#atan2(arma.Mat, arma.Mat)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testAtan2() throws IOException {
    Mat input = new Mat();
    input.load("./test/data/input/trigonometric.mat");
    
    Mat expected = new Mat();
    expected.load("./test/data/expected/util/TestTrigonometry/testAtan2.mat");
    assertMatElementWiseEquals("", expected, Trigonometry.atan2(input.row(0), input.row(1)));
  }

}
