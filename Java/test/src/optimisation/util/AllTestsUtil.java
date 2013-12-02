package optimisation.util;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

/**
 * @author Sebastian Niemann <niemann@sra.uni-hannovr.de>
 */
@RunWith(Suite.class)
@SuiteClasses({
  TestAlgebraCross.class,
  TestAlgebraNorm.class,
  TestGeometry2DRotationMatrix.class,
  TestGeometryTaitBryanAngles.class,
  TestTrigonometry.class
})
public class AllTestsUtil {

}
