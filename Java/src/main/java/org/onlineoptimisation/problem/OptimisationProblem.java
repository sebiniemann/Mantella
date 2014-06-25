package org.onlineoptimisation.problem;

import org.armadillojava.Arma;
import org.armadillojava.Mat;
import org.armadillojava.Col;

public abstract class OptimisationProblem {

  protected int    _numberOfDimensions;

  protected Col    _lowerBounds;
  protected Col    _upperBounds;

  protected int    _maximalNumberOfEvaluations;
  protected double _acceptableObjectiveValue;

  protected Col    _parameterShift;
  protected Mat    _parameterRotation;

  protected double _objectiveValueShift;

  protected int    _numberOfEvaluations;

  public OptimisationProblem(int numberOfDimensions) {
    _numberOfDimensions = numberOfDimensions;

    _lowerBounds = Arma.ones(Col.class, _numberOfDimensions).times(-Double.MAX_VALUE);
    _upperBounds = Arma.ones(Col.class, _numberOfDimensions).times(Double.MAX_VALUE);

    _maximalNumberOfEvaluations = 1000;
    _acceptableObjectiveValue = -Double.MAX_VALUE;

    _parameterShift = Arma.zeros(Col.class, numberOfDimensions);
    _parameterRotation = Arma.eye(numberOfDimensions, numberOfDimensions);

    _objectiveValueShift = 0;

    _numberOfEvaluations = 0;
  }

  public final int getNumberOfDimensions() {
    return _numberOfDimensions;
  }

  /**
   * Sets the maximal number of evaluations of the objective function.
   * 
   * @param maximalNumberOfEvaluations The maximal number of evaluations
   * 
   * @throws RuntimeException The maximal number of function evaluations ({@code maximalNumberOfEvaluations}) must be greater than 0.
   */
  public final void setMaximalNumberOfEvaluations(int maximalNumberOfEvaluations) throws RuntimeException {
    if(maximalNumberOfEvaluations < 1) {
      throw new RuntimeException("The maximal number of function evaluations (" + maximalNumberOfEvaluations + ") must be greater than 0.");
    }
    
    _maximalNumberOfEvaluations = maximalNumberOfEvaluations;
  }

  /**
   * Returns the maximal number of evaluations of the objective function.
   */
  public final int getMaximalNumberOfEvaluations() {
    return _maximalNumberOfEvaluations;
  }

  public final void setAcceptableObjectiveValue(double acceptableObjectiveValue) {
    _acceptableObjectiveValue = acceptableObjectiveValue;
  }

  public final double getAcceptableObjectiveValue() {
    return _acceptableObjectiveValue;
  }
  
  /**
   * 
   * @param upperBounds The upper bound
   * 
   * @throws RuntimeException The dimension ({@code parameterShift.n_elem}) of the lower bound must match the
   *           dimension of the problem ({@code _numberOfDimensions}).
   */

  public final void setLowerBounds(Col lowerBounds) throws RuntimeException {
    if (lowerBounds.n_elem != _numberOfDimensions) {
      throw new RuntimeException("The dimension (" + lowerBounds.n_elem + ") of the lower bound must match the dimension of the problem (" + _numberOfDimensions + ").");
    }

    _lowerBounds = lowerBounds;
  }

  public final Col getLowerBounds() {
    return _lowerBounds;
  }
  
  /**
   * 
   * @param upperBounds The upper bound
   * 
   * @throws RuntimeException The dimension ({@code parameterShift.n_elem}) of the upper bound must match the
   *           dimension of the problem ({@code _numberOfDimensions}).
   */
  public final void setUpperBounds(Col upperBounds) throws RuntimeException {
    if (upperBounds.n_elem != _numberOfDimensions) {
      throw new RuntimeException("The dimension (" + upperBounds.n_elem + ") of the upper bound must match the dimension of the problem (" + _numberOfDimensions + ").");
    }

    _upperBounds = upperBounds;
  }

  public final Col getUpperBounds() {
    return _upperBounds;
  }

  /**
   * 
   * @param parameterShift The parameter shift
   * 
   * @throws RuntimeException The dimension ({@code parameterShift.n_elem}) of the parameter shift must match the
   *           dimension of the problem ({@code _numberOfDimensions}).
   */
  public final void setParameterShift(Col parameterShift) {
    if (parameterShift.n_elem != _numberOfDimensions) {
      throw new RuntimeException("The dimension (" + parameterShift.n_elem + ") of the parameter shift must match the dimension of the problem (" + _numberOfDimensions + ").");
    }

    _parameterShift = parameterShift;
  }

  public final void setObjectiveValueShift(double objectiveValueShift) {
    _objectiveValueShift = objectiveValueShift;
  }
  
  /**
   * Returns the number of counted objective function.
   */
  public final int getNumberOfEvaluations() {
    return _numberOfEvaluations;
  }

  /**
   * Sets the left hand-side rotation matrix used to rotate the parameter space around (0, .., 0) +
   * {@code parameterShift}.
   * 
   * @param parameterRotation The matrix
   * 
   * @throws RuntimeException The ({@code n_rows}, {@code n_cols})-matrix must be square and its number of rows/columns
   *           must match the dimension of the problem ({@code _numberOfDimensions}).
   * @throws The determinant ({@code determinant}) of a proper rotation matrix must be 1.
   * @throws The matrix must be orthogonal.
   */
  public final void setRotationMatrix(Mat parameterRotation) throws RuntimeException {
    if (!parameterRotation.is_square() || parameterRotation.n_rows != _numberOfDimensions) {
      throw new RuntimeException("The (" + parameterRotation.n_rows + ", " + parameterRotation.n_cols + ")-matrix must be square and its number of rows/columns must match the dimension of the problem (" + _numberOfDimensions + ").");
    }

    double determinant = Arma.det(parameterRotation);
    if (determinant != 1) {
      throw new RuntimeException("The determinant (" + determinant + ") of a proper rotation matrix must be 1.");
    }

    if (Arma.any(Arma.vectorise(Col.class, parameterRotation.t().nonEquals(parameterRotation).i()))) {
      throw new RuntimeException("The matrix must be orthogonal.");
    }

    _parameterRotation = parameterRotation;
  }

  /**
   * Resets the counted number of evaluations to 0.
   */
  public final void resetNumberOfEvaluations() {
    _numberOfEvaluations = 0;
  }

  /**
   * Returns true if the provided parameter did not violate any constraints.
   * 
   * @param parameter The parameter
   * 
   * @throws RuntimeException The dimension of the parameter ({@code parameter.n_elem}) must match the dimension of the
   *           problem ({@code _numberOfDimensions}).
   */
  public final boolean isSatisfyingConstraints(Col parameter) throws RuntimeException {
    if (parameter.n_rows != _numberOfDimensions) {
      throw new RuntimeException("The dimension of the parameter (" + parameter.n_elem + ") must match the dimension of the problem (" + _numberOfDimensions + ").");
    }

    for (int n = 0; n < parameter.n_elem; n++) {
      double value = parameter.at(n);

      if (value < _lowerBounds.at(n) || value > _upperBounds.at(n)) {
        return false;
      }
    }

    return true;
  }

  /**
   * Returns the objective value of the provided parameter.
   * 
   * @param parameter The parameter
   */
  protected abstract double getObjectiveValueImplementation(Col parameter);

  /**
   * Returns the objective value of the provided parameter and increases the number of function evaluations by 1.
   * 
   * @param parameter The parameter
   * 
   * @throws RuntimeException The dimension ({@code parameter.n_elem}) of the parameter must match the dimension of the
   *           problem ({@code _numberOfDimensions}).
   */
  public final double getObjectiveValue(Col parameter) throws RuntimeException {
    if (parameter.n_rows != _numberOfDimensions) {
      throw new RuntimeException("The dimension (" + parameter.n_elem + ") of the parameter must match the dimension of the problem (" + _numberOfDimensions + ").");
    }

    _numberOfEvaluations++;

    return getObjectiveValueImplementation(_parameterRotation.times(parameter.minus(_parameterShift))) + getSoftConstraintValueImplementation(parameter) + _objectiveValueShift;
  }

  protected double getSoftConstraintValueImplementation(Col parameter) {
    return 0;
  };
  
  public final double getSoftConstraintValue(Col parameter) {
    if (parameter.n_rows != _numberOfDimensions) {
      throw new RuntimeException("The dimension (" + parameter.n_elem + ") of the parameter must match the dimension of the problem (" + _numberOfDimensions + ").");
    }
    
    return getSoftConstraintValueImplementation(parameter);
  }
  
}
