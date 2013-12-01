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

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

/**
 * @author Sebastian Niemann <niemann@sra.uni-hannovr.de>
 */
@RunWith(Suite.class)
@SuiteClasses({
  TestUtilAlgebraCross.class,
  TestUtilAlgebraNorm.class
})
public class AllTests {

}
