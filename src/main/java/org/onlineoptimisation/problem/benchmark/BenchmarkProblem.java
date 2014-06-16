package org.onlineoptimisation.problem.benchmark;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.onlineoptimisation.problem.OptimisationProblem;

/**
 * Created by romeoshuka on 12.06.14.
 */

public abstract class BenchmarkProblem extends OptimisationProblem {

    public BenchmarkProblem(int numberOfDimensions) {
        super(numberOfDimensions);
        setLowerBounds(Arma.zeros(Col.class, _numberOfDimensions).minus(5));
        setUpperBounds(Arma.zeros(Col.class, _numberOfDimensions).plus(5));
    }


}
