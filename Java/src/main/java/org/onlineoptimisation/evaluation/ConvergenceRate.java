package org.onlineoptimisation.evaluation;

import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Paths;

import org.onlineoptimisation.DefaultMonitorType;
import org.onlineoptimisation.Statistic;
import org.onlineoptimisation.Storage;
import org.onlineoptimisation.Monitor;
import org.onlineoptimisation.Visulisation;
import org.onlineoptimisation.algorithm.OptimisationAlgorithm;
import org.onlineoptimisation.problem.OptimisationProblem;

public class ConvergenceRate extends Evaluation {

  protected OptimisationProblem   _optimisationProblem;
  protected OptimisationAlgorithm _optimisationAlgorithm;

  protected PrintWriter           _writer;

  public ConvergenceRate(OptimisationProblem optimisationProblem, OptimisationAlgorithm optimisationAlgorithm) {
    _optimisationProblem = optimisationProblem;
    _optimisationAlgorithm = optimisationAlgorithm;
  }

  @Override
  public void compute() throws IOException {
    Monitor.observe(DefaultMonitorType.CurrentBestObjectiveValue);

    Statistic statistic = new Statistic();
    for (int n = 0; n < 100; n++) {
      _optimisationAlgorithm.optimise(_optimisationProblem);
      statistic.update(Monitor.get(DefaultMonitorType.CurrentBestObjectiveValue));

      Monitor.clear(DefaultMonitorType.CurrentBestObjectiveValue);
    }

    Storage.CSV(Paths.get("../gnuplot/convergenceRateMean.dat"), statistic.getMean());
    Storage.CSV(Paths.get("../gnuplot/convergenceRateStdandardDeviation.dat"), statistic.getStandardDeviation());

    Visulisation.plot("plotConvergenceRate.txt");
  }
}