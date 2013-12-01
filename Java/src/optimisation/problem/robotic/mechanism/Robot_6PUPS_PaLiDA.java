/*******************************************************************************
 * Copyright (c) 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
 *
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/MIT
 *******************************************************************************/
package optimisation.problem.robotic.mechanism;

import arma.Mat;

/**
 * A 6(P̲)UP̲S robot called PaLiDA, as build by the Institute of Production Engineering and Machine Tools, Leibniz
 * Universität Hannover.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
public class Robot_6PUPS_PaLiDA extends Robot_NPUPS {

  /**
   * Specifies the parameter for an actual 6(P̲)UP̲S robot.
   */
  public Robot_6PUPS_PaLiDA() {
    super(
      // {X-axis, Y-axis, Z-axis}
      new Mat(new double[][]{
        {-0.025561381023353, 0.086293776138137, 0.120000000000000}, // First kinematic chain
        {0.025561381023353, 0.086293776138137, 0.120000000000000}, // Second kinematic chain
        {0.087513292835791, -0.021010082747031, 0.120000000000000}, // Third kinematic chain
        {0.061951911812438, -0.065283693391106, 0.120000000000000}, // Forth kinematic chain
        {-0.061951911812438, -0.065283693391106, 0.120000000000000}, // Fifth kinematic chain
        {-0.087513292835791, -0.021010082747032, 0.120000000000000} // Sixth kinematic chain
      }),
      // {X-axis, Y-axis, Z-axis}
      new Mat(new double[][]{
        {-0.290503789274734, 0.317029254828353, 0.2}, // First kinematic chain
        {0.290503789274734, 0.317029254828353, 0.2}, // Second kinematic chain
        {0.419807283061571, 0.093069033993384, 0.2}, // Third kinematic chain
        {0.129303493786838, -0.410098288821738, 0.2}, // Forth kinematic chain
        {-0.129303493786837, -0.410098288821738, 0.2}, // Fifth kinematic chain
        {-0.419807283061571, 0.093069033993384, 0.2} // Sixth kinematic chain
      }),
      // {X-axis, Y-axis, Z-axis}
      new Mat(new double[][]{
        {-0.290503789274734, 0.317029254828353, -0.6}, // First kinematic chain
        {0.290503789274734, 0.317029254828353, -0.6}, // Second kinematic chain
        {0.419807283061571, 0.093069033993384, -0.6}, // Third kinematic chain
        {0.129303493786838, -0.410098288821738, -0.6}, // Forth kinematic chain
        {-0.129303493786837, -0.410098288821738, -0.6}, // Fifth kinematic chain
        {-0.419807283061571, 0.093069033993384, -0.6} // Sixth kinematic chain
      }));
  }
}
