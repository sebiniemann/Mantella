package org.onlineoptimisation.algorithm;

import java.util.ArrayList;
import java.util.List;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Op;

public class HookeAlgorithm extends OptimisationAlgorithm {

  protected Col     _basePoint;
  protected double  _basePointObjectiveValue;

  protected Col     _stepSize;
  protected boolean _reduceStepSize;

  @Override
  protected void initialisation() {
    _basePoint = Arma.randu(Col.class, _optimisationProblem.getNumberOfDimensions());
    _basePoint.inPlace(Op.ELEMTIMES, _optimisationProblem.getUpperBounds().minus(_optimisationProblem.getLowerBounds()));
    _basePoint.inPlace(Op.PLUS, _optimisationProblem.getLowerBounds());
    
    _basePointObjectiveValue = _optimisationProblem.getObjectiveValue(_basePoint);

    _stepSize = getOptimisationProblem().getUpperBounds().minus(getOptimisationProblem().getLowerBounds());
    _reduceStepSize = false;
  }

  @Override
  protected List<Col> getNextParameters() {
    if (_reduceStepSize) {
      _stepSize = _stepSize.divide(2);
    }

    List<Col> nextParameters = new ArrayList<>();
    for (int dimension = 0; dimension < getOptimisationProblem().getNumberOfDimensions(); dimension++) {
      Col candidatePoint = new Col(_basePoint);
      candidatePoint.at(dimension, Op.PLUS, _stepSize.at(dimension));
      nextParameters.add(candidatePoint);
      candidatePoint.at(dimension, Op.MINUS, _stepSize.at(dimension) * 2);
      nextParameters.add(candidatePoint);
    }

    return nextParameters;
  }

  @Override
  protected void updateInformations(List<Col> parameters, List<Double> objectiveValues, List<Boolean> isSatisfyingConstraints) {
    for(int n = 0; n < parameters.size(); n++) {
      if (isSatisfyingConstraints.get(n) && objectiveValues.get(n) < _basePointObjectiveValue) {
        _basePoint = parameters.get(n);
        _basePointObjectiveValue = objectiveValues.get(n);

        _reduceStepSize = false;
      }
      else {
        _reduceStepSize = true;
      }
    }
  }
}
