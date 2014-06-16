package org.onlineoptimisation.evaluation;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;

import org.armadillojava.Arma;
import org.armadillojava.Col;

public class FitnessFunction extends Evaluation {

  public void compute() throws IOException {
    Col X = Arma.linspace(Col.class, -5, 5, 30);
    Col Y = Arma.linspace(Col.class, -5, 5, 30);

    PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(new FileOutputStream("../gnuplot/data.txt", false), StandardCharsets.UTF_8)));
    writer.println("# X Y Z");

    for (int y = 0; y < Y.n_elem; y++) {
      for (int x = 0; x < X.n_elem; x++) {
        double xx = X.at(x);
        double yy = Y.at(y);
        
        writer.println(xx + " " + yy + " " + (Math.pow(xx, 2) + Math.pow(yy, 2)));
      }
      writer.println();
    }
    
    writer.close();
    
    ProcessBuilder processBuilder = new ProcessBuilder("gnuplot", "plot.txt");
    processBuilder.directory(new File("../gnuplot"));
    processBuilder.start();
  }
}
