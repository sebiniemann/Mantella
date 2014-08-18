/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package visualisation;

import org.armadillojava.Mat;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

/**
 *
 * @author Wolff
 */
public class CanvasTest {

  public static void main(String[] args) throws InterruptedException {
    // {X-axis, Y-axis}
    Mat pjStart = new Mat(new double[][]{
      {0.090782289181018, 1.001794428720682}, // First kinematic chain
      {0.0, -0.2}, // Second kinematic chain
      {1.1996, -0.2} // Third kinematic chain
    });
    // {X-axis, Y-axis},
    Mat pjEnd = new Mat(new double[][]{
      {1.090757710818982, 1.008805571279318}, // First kinematic chain
      {0.0, 0.8}, // Second kinematic chain
      {1.1996, 0.8} // Third kinematic chain
    });
    Mat pjLength = new Mat(new double[][]{
      {1},
      {1},
      {1}
    });
    Mat arms = new Mat(new double[][]{
      {0.59077, 1.0053, 0.008968319474372, 0.985164781164465, 0.499933419554166, 0.606954081945581},
      {0, 0, 0.560700952618889, -0.154207497004102, 0.407248290222917, 0.446522959027210},
      {1.1996, 0, 1.197643899206675, 0.581206708297217, 0.592818290222917, 0.446522959027210}
    });
    Mat activeJoints = new Mat(new double[][]{
      {1, 0, 0},
      {1, 0, 0},
      {1, 0, 0}
    });
    Mat linkLength = new Mat(new double[][]{
      {0.58, 0.61},
      {0.58, 0.62},
      {0.58, 0.62}
    });

    Mat passiveRotEndEffector = new Mat(new double[][]{
      {0.499933419554166, 0.606954081945581},
      {0.407248290222917, 0.446522959027210},
      {0.592818290222917, 0.446522959027210}
    });

    Mat endEffectorPos = new Mat(new double[][]{
      {0.5f},
      {0.5f},
      {0}
    });

    double endEffectorRot = 0.5;

    Mat examplePlotMatrix = new Mat(new double[][]{
      {-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1},
      {-0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 0.75},
      {-0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 0.75, 0.5},
      {0.25, 0, 0.25, 0.5, 0.75, 1, 0.75, 0.5, 0.25},
      {0, 0.25, 0.5, 0.75, 1, 0.75, 0.5, 0.25, 0},
      {0.25, 0.5, 0.75, 1, 0.75, 0.5, 0.25, 0, -0.25},
      {0.5, 0.75, 1, 0.75, 0.5, 0.25, 0, -0.25, -0.5},
      {0.75, 1, 0.75, 0.5, 0.25, 0, -0.25, -0.5, -0.75},
      {1, 0.75, 0.5, 0.25, 0, -0.25, -0.5, -0.75, -1}
    });

    JMonkeyApplication app = new JMonkeyApplication(640, 480, pjStart, pjEnd, pjLength, arms, activeJoints, linkLength, endEffectorPos, endEffectorRot);

    app.setPlotMatrix(examplePlotMatrix);

    JFrame frame = new JFrame("testy");
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    JPanel first = new JPanel();
    first.add(new JLabel("ich bin ein label."));
    first.add(new JLabel("ich bin auch ein label!"));
    first.add(app.get3DCanvas());
    first.add(app.getPlotCanvas());
    frame.add(first);
    frame.pack();
    frame.setLocationRelativeTo(null);
    frame.setVisible(true);
    app.startCanvas();

  }
}
