package org.onlineoptimisation;

import org.onlineoptimisation.evaluation.Evaluation;
import org.onlineoptimisation.evaluation.FitnessFunction;
import org.onlineoptimisation.problem.OptimisationProblem;
import org.onlineoptimisation.problem.benchmark.AttractiveSectorFunction;
import org.onlineoptimisation.problem.benchmark.BentCigarFunction;
import org.onlineoptimisation.problem.benchmark.BuecheRastriginFunction;
import org.onlineoptimisation.problem.benchmark.DiscusFunction;
import org.onlineoptimisation.problem.benchmark.EllipsoidalFunction;
import org.onlineoptimisation.problem.benchmark.LinearSlope;
import org.onlineoptimisation.problem.benchmark.RastriginFunction;

public class Main {

  public static void main(String[] args) throws Exception {
    OptimisationProblem optimisationProblem = new BentCigarFunction(2);
    Evaluation fitnessFunction = new FitnessFunction(optimisationProblem);
    fitnessFunction.compute();

    // OptimisationProblem sphereFunction = new SphereFunction(2);
    // sphereFunction.setLowerBounds(Arma.zeros(Col.class, sphereFunction.getNumberOfDimensions()).minus(5));
    // sphereFunction.setUpperBounds(Arma.zeros(Col.class, sphereFunction.getNumberOfDimensions()).plus(5));
    // sphereFunction.setMaximalNumberOfEvaluations(250);
    //
    // OptimisationAlgorithm standardParticleSwarmOptimisation2011 = new StandardParticleSwarmOptimisation2011();
    // Map<String, Object> parameterisation = new HashMap<>();
    // parameterisation.put("populationSize", 20);
    // standardParticleSwarmOptimisation2011.setParameterisation(parameterisation);
    //
    // Evaluation convergenceRate = new ConvergenceRate(sphereFunction, standardParticleSwarmOptimisation2011);
    // convergenceRate.compute();

    // List<OptimisationProblem> optimisationProblems = new ArrayList<>();
    // optimisationProblems.add(new SphereFunction(2));
    // optimisationProblems.add(new MaxZeros(2));
    // optimisationProblems.add(new RastriginFunction(2));
    //
    // List<OptimisationAlgorithm> optimisationAlgorithms = new ArrayList<>();
    // optimisationAlgorithms.add(new HillClimbing());
    // optimisationAlgorithms.add(new HookeAlgorithm());
    // optimisationAlgorithms.add(new StandardParticleSwarmOptimisation2011());
    //
    // Evaluation performanceProfile = new PerformanceProfile(optimisationProblems, optimisationAlgorithms);
    // performanceProfile.compute();
  }

}
