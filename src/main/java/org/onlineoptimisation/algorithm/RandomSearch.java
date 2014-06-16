package org.onlineoptimisation.algorithm;

import java.util.ArrayList;
import java.util.List;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Op;

public class RandomSearch extends OptimisationAlgorithm {

  @Override
  protected List<Col> getNextParameters() {
    List<Col> nextParameters = new ArrayList<>();

    Col parameter = Arma.randu(Col.class, _optimisationProblem.getNumberOfDimensions());
    parameter.inPlace(Op.ELEMTIMES, _optimisationProblem.getUpperBounds().minus(_optimisationProblem.getLowerBounds()));
    parameter.inPlace(Op.PLUS, _optimisationProblem.getLowerBounds());
    
    nextParameters.add(parameter);
    return nextParameters;
  }
}
