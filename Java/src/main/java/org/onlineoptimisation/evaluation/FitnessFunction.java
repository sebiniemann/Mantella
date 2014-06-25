package org.onlineoptimisation.evaluation;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.onlineoptimisation.Visulisation;
import org.onlineoptimisation.problem.OptimisationProblem;

public class FitnessFunction extends Evaluation {

  OptimisationProblem _optimisationProblem;
  
  public FitnessFunction(OptimisationProblem optimisationproblem) {
    _optimisationProblem = optimisationproblem;
  }

  public void compute() throws IOException {
    Col X = Arma.linspace(Col.class, -5, 5, 250);
    Col Y = Arma.linspace(Col.class, -5, 5, 250);

    PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(new FileOutputStream("../gnuplot/fitnessFunction.dat", false), StandardCharsets.UTF_8)));
    writer.println("# X Y Z");

    for (int y = 0; y < Y.n_elem; y++) {
      for (int x = 0; x < X.n_elem; x++) {
        double xx = X.at(x);
        double yy = Y.at(y);
        
        writer.println(xx + " " + yy + " " + _optimisationProblem.getObjectiveValue(new Col(new double[]{xx, yy})));
      }
      writer.println();
    }
    
    writer.close();
    
    Visulisation.plot("plotFitnessFunction.txt");
  }
}
