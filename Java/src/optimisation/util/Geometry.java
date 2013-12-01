/*******************************************************************************
 * Copyright (c) 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
 *
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/MIT
 *******************************************************************************/
package optimisation.util;

import arma.Mat;

/**
 * Provides geometry helper functions.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
public class Geometry {
  /**
   * Returns a two-dimensional rotation matrix through an angle
   * 
   * @param angle The angle
   * @return The rotation matrix
   */
  public static Mat get2DRotationMatrix(double angle) {
    double sinAngle = Math.sin(angle);
    double cosAngle = Math.cos(angle);

    return new Mat(new double[][]{
      {cosAngle, -sinAngle},
      {sinAngle, cosAngle}
    });
  }

  /**
   * Returns a ZYX-Tait-Bryan angles rotation matrix
   * 
   * @param rollAngle The roll angle
   * @param pitchAngle The pitch angle
   * @param yawAngle The yaw angle
   * @return The rotation matrix
   */
  public static Mat getZYXTaitBryanAnglesRotationMatrix(double rollAngle, double pitchAngle, double yawAngle) {
    double sinRollAngle = Math.sin(rollAngle);
    double cosRollAngle = Math.cos(rollAngle);
    double sinPitchAngle = Math.sin(pitchAngle);
    double cosPitchAngle = Math.cos(pitchAngle);
    double sinYawAngle = Math.sin(yawAngle);
    double cosYawAngle = Math.cos(yawAngle);

    return new Mat(new double[][]{
      {cosYawAngle * cosPitchAngle, cosYawAngle * sinPitchAngle * sinRollAngle - sinYawAngle * cosRollAngle, cosYawAngle * sinPitchAngle * cosRollAngle + sinYawAngle * sinRollAngle},
      {sinYawAngle * cosPitchAngle, sinYawAngle * sinPitchAngle * sinRollAngle - cosYawAngle * cosRollAngle, sinYawAngle * sinPitchAngle * cosRollAngle - cosYawAngle * sinRollAngle},
      {-sinPitchAngle, cosPitchAngle * sinRollAngle, cosPitchAngle * cosRollAngle}
    });
  }
}
