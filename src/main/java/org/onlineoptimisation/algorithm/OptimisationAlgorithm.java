package org.onlineoptimisation.algorithm;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.onlineoptimisation.Monitor;
import org.onlineoptimisation.DefaultMonitorType;
import org.onlineoptimisation.problem.OptimisationProblem;
import org.armadillojava.Col;

public abstract class OptimisationAlgorithm {

  protected OptimisationProblem _optimisationProblem;

  private Col                 _bestParameter;
  private double              _bestObjectiveValue;

  protected void initialisation() {};

  protected abstract List<Col> getNextParameters();

  protected void updateInformations(List<Col> parameters, List<Double> objectiveValues, List<Boolean> isSatisfyingConstraints) {};

  /**
   * 
   * @throws RuntimeException The dimension of the parameter ({@code parameter.n_elem}) must match the dimension of the
   *           problem ({@code _numberOfDimensions}).
   */
  public final void optimise(OptimisationProblem optimisationProblem) throws RuntimeException {
    
    _optimisationProblem = optimisationProblem;
    _bestObjectiveValue = Double.POSITIVE_INFINITY;

    int numberOfIterations = 1;
    optimisationProblem.resetNumberOfEvaluations();

    Monitor.add(DefaultMonitorType.CurrentNumberOfIterations, numberOfIterations);
    initialisation();

    while (!isFinished() && !isTerminated()) {
      List<Col> parameters = getNextParameters();
      List<Double> objectiveValues = new ArrayList<>();
      List<Boolean> isSatisfyingConstraintsList = new ArrayList<>();

      numberOfIterations++;
      Monitor.add(DefaultMonitorType.CurrentNumberOfIterations, numberOfIterations);

      for (Col parameter : parameters) {
        /*
         * The number of dimensions of each parameter is validated within _optimisationProblem.getObjectiveValue(Col).
         */
        
        double objectiveValue = _optimisationProblem.getObjectiveValue(parameter);
        boolean isSatisfyingConstraints = _optimisationProblem.isSatisfyingConstraints(parameter);

        if (isSatisfyingConstraints && objectiveValue < _bestObjectiveValue) {
          _bestParameter = parameter;
          _bestObjectiveValue = objectiveValue;
        }
        
        Monitor.add(DefaultMonitorType.CurrentBestParameter, _bestParameter);
        Monitor.add(DefaultMonitorType.CurrentBestObjectiveValue, _bestObjectiveValue);

        if (isFinished() || isTerminated()) {
          Monitor.add(DefaultMonitorType.FinalNumberOfIterations, numberOfIterations);
          Monitor.add(DefaultMonitorType.FinalNumberOfFunctionEvaluations, _optimisationProblem.getNumberOfEvaluations());
          return;
        }
        
        objectiveValues.add(objectiveValue);
        isSatisfyingConstraintsList.add(isSatisfyingConstraints);
      }
      
      updateInformations(parameters, objectiveValues, isSatisfyingConstraintsList);
    }
  }

  public final OptimisationProblem getOptimisationProblem() {
    return _optimisationProblem;
  }
  
  protected final boolean isFinished() {
    return (_bestObjectiveValue <= _optimisationProblem.getAcceptableObjectiveValue());
  }

  protected final boolean isTerminated() {
    return (_optimisationProblem.getNumberOfEvaluations() >= _optimisationProblem.getMaximalNumberOfEvaluations());
  }

  public void setParameterisation(Map<String, Object> parameters) throws RuntimeException {}
}
