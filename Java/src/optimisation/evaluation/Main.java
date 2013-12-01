/*******************************************************************************
 * Copyright (c) 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
 *
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/MIT
 *******************************************************************************/
package optimisation.evaluation;

import arma.Mat;
import optimisation.problem.robotic.mechanism.Robot;
import optimisation.problem.robotic.mechanism.Robot_3PRRR_IMES;
import optimisation.problem.robotic.mechanism.Robot_3PUPS;
import optimisation.problem.robotic.mechanism.Robot_6PUPS_PaLiDA;

/**
 * Test class
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
public class Main {

  /**
   * Test method
   * 
   * @param args wayne
   */
  public static void main(String[] args) {
    Robot robot_3PRRR_IMES = new Robot_3PRRR_IMES();
    System.out.println(robot_3PRRR_IMES.getJacobian(new Mat(new double[][]{{0.5, 0.5}, {0.5, 0.5}, {0.5, 0.5}}), new Mat(new double[][]{{0, 0}}), 0, 0, 0));

    Robot robot_6PUPS_PaLiDA = new Robot_6PUPS_PaLiDA();
    System.out.println(robot_6PUPS_PaLiDA.getJacobian(new Mat(new double[][]{{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}}), new Mat(new double[][]{{0, 0, 0}}), 0, 0, 0));

    Robot robot_3PUPS = new Robot_3PUPS();
    System.out.println(robot_3PUPS.getJacobian(new Mat(new double[][]{{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}}), new Mat(new double[][]{{0, 0, 0}}), 0, 0, 0));
  }
}
