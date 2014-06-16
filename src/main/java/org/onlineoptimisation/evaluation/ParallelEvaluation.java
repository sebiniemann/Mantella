package org.onlineoptimisation.evaluation;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.onlineoptimisation.DefaultMonitorType;
import org.onlineoptimisation.Monitor;
import org.onlineoptimisation.algorithm.OptimisationAlgorithm;
import org.onlineoptimisation.algorithm.StandardParticleSwarmOptimisation2011;
import org.onlineoptimisation.problem.OptimisationProblem;
import org.onlineoptimisation.problem.benchmark.SphereFunction;

import java.util.List;

/**
 * Created by romeoshuka on 11.06.14.
 */
public class ParallelEvaluation extends Evaluation {

    @Override
    public void compute() throws Exception {

        //Read the Variables from the Monitor
        Monitor.observe(DefaultMonitorType.FinalNumberOfFunctionEvaluations, DefaultMonitorType.FinalNumberOfIterations, DefaultMonitorType.CurrentNumberOfIterations, DefaultMonitorType.CurrentNumberOfFunctionEvaluations);


        //Jetzt kommt die Optimierung
        OptimisationProblem sphereFunction = new SphereFunction(2);
        sphereFunction.setMaximalNumberOfEvaluations(250);
        sphereFunction.setAcceptableObjectiveValue(0.01);

        OptimisationAlgorithm pso = new StandardParticleSwarmOptimisation2011();
        pso.optimise(sphereFunction);

        int evaluations = (int) Monitor.get(DefaultMonitorType.FinalNumberOfFunctionEvaluations).get(0);
        int iterations = (int) Monitor.get(DefaultMonitorType.FinalNumberOfIterations).get(0);
        System.out.println("Iterations: " + iterations);
        System.out.println("Evaluation: " + evaluations);

        List<List<Object>> myList = Monitor.get(DefaultMonitorType.CurrentNumberOfIterations, DefaultMonitorType.CurrentNumberOfFunctionEvaluations);

        for(List<Object> iterationsList : myList) {
            for(Object iterationObject : iterationsList) {
                System.out.print(" " + iterationObject + " : ");
            }
            System.out.println();
        }
    }
}
