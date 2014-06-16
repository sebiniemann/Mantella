package org.onlineoptimisation.algorithm;

import java.util.ArrayList;
import java.util.List;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Op;

public class HillClimbing extends OptimisationAlgorithm {

  protected Col _parameter;
  protected double _objectiveValue;
  
  @Override
  protected void initialisation() {
    _parameter = Arma.randu(Col.class, _optimisationProblem.getNumberOfDimensions());
    _parameter.inPlace(Op.ELEMTIMES, _optimisationProblem.getUpperBounds().minus(_optimisationProblem.getLowerBounds()));
    _parameter.inPlace(Op.PLUS, _optimisationProblem.getLowerBounds());
    
    _objectiveValue = _optimisationProblem.getObjectiveValue(_parameter);
  }

  @Override
  protected List<Col> getNextParameters() {
    List<Col> nextParameters = new ArrayList<>();
    
    Col step = Arma.randn(Col.class, _optimisationProblem.getNumberOfDimensions());
    step.inPlace(Op.DIVIDE, Arma.norm(step, 2));
    step.inPlace(Op.TIMES, Arma.as_scalar(Arma.randu(1, 1)));
    
    nextParameters.add(_parameter.plus(step));
    
    return nextParameters;
  }

  @Override
  protected void updateInformations(List<Col> parameters, List<Double> objectiveValues, List<Boolean> isSatisfyingConstraints) {
    for(int n = 0; n < parameters.size(); n++) {
      if (isSatisfyingConstraints.get(n) && objectiveValues.get(n) < _objectiveValue) {
        _parameter = parameters.get(n);
        _objectiveValue = objectiveValues.get(n);
      }
    }
  }
}
