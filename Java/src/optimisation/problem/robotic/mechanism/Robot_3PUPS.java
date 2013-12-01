package optimisation.problem.robotic.mechanism;

import arma.Mat;

/**
 * A theoretical 3(P̲)UP̲S robot based on the 6(P̲)UP̲S called PaLiDA, as build by the Institute of Production Engineering and
 * Machine Tools, Leibniz Universität Hannover.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
public class Robot_3PUPS extends Robot_NPUPS {

  /**
   * Specifies the parameter for an theoretical 3(P̲)UP̲S robot.
   */
  public Robot_3PUPS() {
    super(
      // {X-axis, Y-axis, Z-axis}
      new Mat(new double[][]{
        {-0.025561381023353, 0.086293776138137, 0.120000000000000}, // First kinematic chain
        {0.087513292835791, -0.021010082747031, 0.120000000000000}, // Second kinematic chain
        {-0.061951911812438, -0.065283693391106, 0.120000000000000}, // Third kinematic chain
      }),
      // {X-axis, Y-axis, Z-axis}
      new Mat(new double[][]{
        {-0.290503789274734, 0.317029254828353, 0.2}, // First kinematic chain
        {0.419807283061571, 0.093069033993384, 0.2}, // Second kinematic chain
        {-0.129303493786837, -0.410098288821738, 0.2}, // Third kinematic chain
      }),
      // {X-axis, Y-axis, Z-axis}
      new Mat(new double[][]{
        {-0.290503789274734, 0.317029254828353, -0.6}, // First kinematic chain
        {0.419807283061571, 0.093069033993384, -0.6}, // Second kinematic chain
        {-0.129303493786837, -0.410098288821738, -0.6}, // Third kinematic chain
      }));
  }
}
