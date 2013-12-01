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
    return new Mat(new double[][]{
      {Math.cos(angle), -Math.sin(angle)},
      {Math.sin(angle), Math.cos(angle)}
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
    return new Mat(new double[][]{
      {Math.cos(yawAngle) * Math.cos(pitchAngle), Math.cos(yawAngle) * Math.sin(pitchAngle) * Math.sin(rollAngle) - Math.sin(yawAngle) * Math.cos(rollAngle), Math.cos(yawAngle) * Math.sin(pitchAngle) * Math.cos(rollAngle) + Math.sin(yawAngle) * Math.sin(rollAngle)},
      {Math.sin(yawAngle) * Math.cos(pitchAngle), Math.sin(yawAngle) * Math.sin(pitchAngle) * Math.sin(rollAngle) - Math.cos(yawAngle) * Math.cos(rollAngle), Math.sin(yawAngle) * Math.sin(pitchAngle) * Math.cos(rollAngle) - Math.cos(yawAngle) * Math.sin(rollAngle)},
      {-Math.sin(pitchAngle), Math.cos(pitchAngle) * Math.sin(rollAngle), Math.cos(pitchAngle) * Math.cos(rollAngle)}
    });
  }
}
