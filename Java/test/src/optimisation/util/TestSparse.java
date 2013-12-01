/**
 * 
 */
package optimisation.util;

import static org.junit.Assert.*;

import java.io.IOException;

import org.junit.Test;

import arma.Mat;

/**
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 *
 */
public class TestSparse {

  /**
   * Test method for {@link optimisation.util.Sparse#nnz(arma.Mat)}.
   * 
   * @throws IOException An I/O error occurred
   */
  @Test
  public void testNnz() throws IOException {
    Mat input = new Mat();
    input.load("./test/data/input/series.mat");
    
    Mat expected = new Mat();
    expected.load("./test/data/expected/util/TestSparse/testNnz.mat");
    assertEquals("", expected.at(0), Sparse.nnz(input), 0);
  }
}
