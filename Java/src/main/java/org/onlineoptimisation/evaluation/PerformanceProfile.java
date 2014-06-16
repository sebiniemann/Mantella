package org.onlineoptimisation.evaluation;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.onlineoptimisation.DefaultMonitorType;
import org.onlineoptimisation.Monitor;
import org.onlineoptimisation.Visulisation;
import org.onlineoptimisation.algorithm.OptimisationAlgorithm;
import org.onlineoptimisation.problem.OptimisationProblem;

public class PerformanceProfile extends Evaluation {

  protected List<OptimisationProblem>   _optimisationProblems;
  protected List<OptimisationAlgorithm> _optimisationAlgorithms;

  public PerformanceProfile(List<OptimisationProblem> optimisationProblems, List<OptimisationAlgorithm> optimisationAlgorithms) {
    _optimisationProblems = optimisationProblems;
    _optimisationAlgorithms = optimisationAlgorithms;
  }

  @Override
  public void compute() throws Exception {
    Monitor.observe(DefaultMonitorType.FinalNumberOfFunctionEvaluations);
    Monitor.observe(DefaultMonitorType.IsFinished);

    Map<OptimisationAlgorithm, List<Double>> performanceRatio = new HashMap<>();
    for (OptimisationAlgorithm optimisationAlgorithm : _optimisationAlgorithms) {
      performanceRatio.put(optimisationAlgorithm, new ArrayList<Double>());
    }

    for (OptimisationProblem optimisationProblem : _optimisationProblems) {
      Map<OptimisationAlgorithm, Integer> absolutePerformance = new HashMap<>();
      
      double baselinePerformance = Integer.MAX_VALUE;
      for (OptimisationAlgorithm optimisationAlgorithm : _optimisationAlgorithms) {
        optimisationAlgorithm.optimise(optimisationProblem);

        int performance;
        if (!((boolean) Monitor.get(DefaultMonitorType.IsFinished).get(0))) {
          performance = Integer.MAX_VALUE;
        } else {
          performance = (int) Monitor.get(DefaultMonitorType.FinalNumberOfFunctionEvaluations).get(0);
        }

        baselinePerformance = Math.max(performance, baselinePerformance);
        absolutePerformance.put(optimisationAlgorithm, performance);
        
        Monitor.clear(DefaultMonitorType.FinalNumberOfFunctionEvaluations);
        Monitor.clear(DefaultMonitorType.IsFinished);
      }
      
      for (OptimisationAlgorithm optimisationAlgorithm : _optimisationAlgorithms) {
        performanceRatio.get(optimisationAlgorithm).add(absolutePerformance.get(optimisationAlgorithm) / baselinePerformance);
      }
    }
    
    Map<OptimisationAlgorithm, Map<Double, Integer>> performanceProfile = new HashMap<>();
    for (OptimisationAlgorithm optimisationAlgorithm : _optimisationAlgorithms) {
//      TreeMap 
    }
    
    Visulisation.plot("plotPerformanceProfile.txt");
  }

}
