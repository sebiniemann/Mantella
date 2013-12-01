package optimisation.problem.robotic.mechanism;

import arma.Mat;

/**
 * A 3(P̲)R̲RR robot, as build by the Institute of Mechatronic Systems, Leibniz Universität Hannover.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
public class Robot_3PRRR_IMES extends Robot_NPRRR {

  /**
   * Specifies the parameter for an actual 3(P̲)R̲RR robot.
   */
  public Robot_3PRRR_IMES() {
    super(
      // {X-axis, Y-axis}
      new Mat(new double[][]{
        {-0.000066580445834, 0.106954081945581}, // First kinematic chain
        {-0.092751709777083, -0.053477040972790}, // Second kinematic chain
        {0.092818290222917, -0.053477040972790} // Third kinematic chain
      }),
      // {Near the base, Near the end effector}
      new Mat(new double[][]{
        {0.58215, 0.61975}, // First kinematic chain
        {0.58152, 0.62002}, // Second kinematic chain
        {0.58121, 0.61964} // Third kinematic chain
      }),
      // {X-axis, Y-axis}
      new Mat(new double[][]{
        {0.090782289181018, 1.001794428720682}, // First kinematic chain
        {0.0, -0.2}, // Second kinematic chain
        {1.1996, -0.2} // Third kinematic chain
      }),
      // {X-axis, Y-axis},
      new Mat(new double[][]{
        {1.090757710818982, 1.008805571279318}, // First kinematic chain
        {0.0, -1.2}, // Second kinematic chain
        {1.1996, -1.2} // Third kinematic chain
      }));
  }
}
