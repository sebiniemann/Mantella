package org.onlineoptimisation;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.onlineoptimisation.algorithm.HillClimbing;
import org.onlineoptimisation.algorithm.HookeAlgorithm;
import org.onlineoptimisation.algorithm.OptimisationAlgorithm;
import org.onlineoptimisation.algorithm.StandardParticleSwarmOptimisation2011;
import org.onlineoptimisation.evaluation.ConvergenceRate;
import org.onlineoptimisation.evaluation.Evaluation;
import org.onlineoptimisation.evaluation.ParallelEvaluation;
import org.onlineoptimisation.evaluation.PerformanceProfile;
import org.onlineoptimisation.problem.OptimisationProblem;
import org.onlineoptimisation.problem.benchmark.MaxZeros;
import org.onlineoptimisation.problem.benchmark.RastriginFunction;
import org.onlineoptimisation.problem.benchmark.SphereFunction;

public class Main {

  public static void main(String[] args) throws Exception {

//    OptimisationProblem sphereFunction = new SphereFunction(2);
//    sphereFunction.setLowerBounds(Arma.zeros(Col.class, sphereFunction.getNumberOfDimensions()).minus(5));
//    sphereFunction.setUpperBounds(Arma.zeros(Col.class, sphereFunction.getNumberOfDimensions()).plus(5));
//    sphereFunction.setMaximalNumberOfEvaluations(250);
//
//    OptimisationAlgorithm standardParticleSwarmOptimisation2011 = new StandardParticleSwarmOptimisation2011();
//    Map<String, Object> parameterisation = new HashMap<>();
//    parameterisation.put("populationSize", 20);
//    standardParticleSwarmOptimisation2011.setParameterisation(parameterisation);
//
//    Evaluation convergenceRate = new ConvergenceRate(sphereFunction, standardParticleSwarmOptimisation2011);
//    convergenceRate.compute();
//
//
//    List<OptimisationProblem> optimisationProblems = new ArrayList<>();
//    optimisationProblems.add(new SphereFunction(2));
//    optimisationProblems.add(new MaxZeros(2));
//    optimisationProblems.add(new RastriginFunction(2));
//
//    List<OptimisationAlgorithm> optimisationAlgorithms = new ArrayList<>();
//    optimisationAlgorithms.add(new HillClimbing());
//    optimisationAlgorithms.add(new HookeAlgorithm());
//    optimisationAlgorithms.add(new StandardParticleSwarmOptimisation2011());
//
//    Evaluation performanceProfile = new PerformanceProfile(optimisationProblems, optimisationAlgorithms);
    //performanceProfile.compute();


      Evaluation evaluation = new ParallelEvaluation();
      evaluation.compute();
  }

}
