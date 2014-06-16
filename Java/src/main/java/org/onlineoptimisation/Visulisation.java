package org.onlineoptimisation;

import java.io.File;
import java.io.IOException;

public class Visulisation {

  public static void plot(String plotSkript) throws IOException {
    ProcessBuilder processBuilder = new ProcessBuilder("gnuplot", plotSkript);
    processBuilder.directory(new File("../gnuplot"));
    processBuilder.start();
  }
  
}
