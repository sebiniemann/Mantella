package org.onlineoptimisation.algorithm;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.onlineoptimisation.DefaultMonitorType;
import org.onlineoptimisation.Monitor;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Mat;
import org.armadillojava.Op;

public class StandardParticleSwarmOptimisation2011 extends OptimisationAlgorithm {

  protected List<Col>    _particles;
  protected List<Col>    _velocities;

  protected int          _populationSize;
  protected double       _neighbourProbability;

  protected double       _acceleration;
  protected double       _localAttraction;
  protected double       _globalAttraction;

  protected List<Col>    _localBestParameters;
  protected List<Double> _localBestObjectiveValue;

  protected Col          _globalBestParameter;
  protected double       _globalBestObjectiveValue;

  protected int          _activeParticle;

  protected Mat          _topology;
  protected boolean      _randomizeTopology;

  public StandardParticleSwarmOptimisation2011() {
    _populationSize = 10;
    _neighbourProbability = Math.pow(1 - 1 / _populationSize, 3);

    _acceleration = 1 / (2 * Math.log(2));
    _localAttraction = 0.5 + Math.log(2);
    _globalAttraction = _localAttraction;
  }
  
  @Override
  public void setParameterisation(Map<String, Object> parameterisation) throws RuntimeException {
    for(Map.Entry<String, Object> parameter : parameterisation.entrySet()) {
      switch(parameter.getKey().toLowerCase()) {
        case "populationsize":
          _populationSize = (int) parameter.getValue();
          break;
        case "neighbourprobability":
          _neighbourProbability = (double) parameter.getValue();
          break;
        case "acceleration":
          _acceleration = (double) parameter.getValue();
          break;
        case "localattraction":
          _localAttraction = (double) parameter.getValue();
          break;
        case "globalattraction":
          _globalAttraction = (double) parameter.getValue();
          break;
        default:
            throw new RuntimeException("Unexpected parameter (" + parameter.getKey() + ").");
      }
    }
  }

  @Override
  protected void initialisation() {
    _particles = new ArrayList<>();
    _velocities = new ArrayList<>();
    _localBestParameters = new ArrayList<>();
    _localBestObjectiveValue = new ArrayList<>();
    _globalBestObjectiveValue = Double.POSITIVE_INFINITY;
    
    for(int n = 0; n < _populationSize; n++) {
      Col particle = Arma.randu(Col.class, _optimisationProblem.getNumberOfDimensions());
      particle.inPlace(Op.ELEMTIMES, _optimisationProblem.getUpperBounds().minus(_optimisationProblem.getLowerBounds()));
      particle.inPlace(Op.PLUS, _optimisationProblem.getLowerBounds());

      Col velocity = Arma.randu(Col.class, _optimisationProblem.getNumberOfDimensions());
      velocity.inPlace(Op.ELEMTIMES, _optimisationProblem.getUpperBounds().minus(_optimisationProblem.getLowerBounds()));
      velocity.inPlace(Op.PLUS, _optimisationProblem.getLowerBounds());
      velocity.inPlace(Op.MINUS, particle);
      
      _particles.add(particle);
      _velocities.add(velocity);
      
      _localBestParameters.add(particle);
      double objectiveValue = getOptimisationProblem().getObjectiveValue(particle);
      _localBestObjectiveValue.add(objectiveValue);
      
      if (objectiveValue < _globalBestObjectiveValue) {
        _globalBestParameter = particle;
        _globalBestObjectiveValue = objectiveValue;
      }

      Monitor.add(DefaultMonitorType.CurrentBestObjectiveValue, _globalBestObjectiveValue);
    }

    _activeParticle = 0;
    _randomizeTopology = true;
  }

  @Override
  protected List<Col> getNextParameters() {
    List<Col> nextParameters = new ArrayList<>();
    
    if (_randomizeTopology) {
      _topology = Arma.eye(_populationSize, _populationSize);

      for (int i = 0; i < _populationSize; i++) {
        for (int j = 0; j < _populationSize; j++) {
          if (i != j && Arma.as_scalar(Arma.randu(1, 1)) < _neighbourProbability) {
            _topology.at(i, j, Op.EQUAL, 1);
          }
        }
      }
    }

    Col particle = _particles.get(_activeParticle);
    Col localBestParameter = _localBestParameters.get(_activeParticle);

    int neighbourhoodBestParticle = 0;
    Col neighbourhoodBestParameter = localBestParameter;
    double neighbourhoodBestObjectiveValue = Double.MAX_VALUE;
    
    for (int n = 0; n < _populationSize; n++) {
      if (_topology.at(_activeParticle, n) == 1 && _localBestObjectiveValue.get(n) < neighbourhoodBestObjectiveValue) {
        neighbourhoodBestParticle = n;
        neighbourhoodBestParameter = _localBestParameters.get(n);
        neighbourhoodBestObjectiveValue = _localBestObjectiveValue.get(n);
      }
    }

    Col localAttractedParameter = particle;
    localAttractedParameter.inPlace(Op.PLUS, (localBestParameter.minus(particle)).times(_localAttraction));
    
    Col neighbourhoodAttractedParameter = particle;
    localAttractedParameter.inPlace(Op.PLUS, (neighbourhoodBestParameter.minus(particle)).times(_localAttraction));
    
    Col attractionCenter;
    if (neighbourhoodBestParticle == _activeParticle) {
      attractionCenter = _particles.get(_activeParticle).plus(localAttractedParameter).divide(2);
    } else {
      attractionCenter = _particles.get(_activeParticle).plus(localAttractedParameter).plus(neighbourhoodAttractedParameter).divide(3);
    }
    
    double attractionRadius = Arma.norm(attractionCenter.minus(_particles.get(_activeParticle)), 2);
    
    Col randomParameter = Arma.randn(Col.class, _optimisationProblem.getNumberOfDimensions());
    randomParameter.inPlace(Op.DIVIDE, Arma.norm(randomParameter, 2));
    randomParameter.inPlace(Op.TIMES, Arma.as_scalar(Arma.randu(1, 1)));
    
    randomParameter.inPlace(Op.TIMES, attractionRadius);
    randomParameter.inPlace(Op.PLUS, attractionCenter);

    Col velocityCandidate = _velocities.get(_activeParticle).times(_acceleration).plus(randomParameter).minus(_particles.get(_activeParticle));
    Col particleCandidate = _particles.get(_activeParticle).plus(velocityCandidate);

    Col belowLowerBound = Arma.find(particleCandidate.strictLessThan(getOptimisationProblem().getLowerBounds()));
    Col aboveUpperBound = Arma.find(particleCandidate.strictGreaterThan(getOptimisationProblem().getUpperBounds()));

    particleCandidate.elem(belowLowerBound, Op.EQUAL, getOptimisationProblem().getLowerBounds().elem(belowLowerBound));
    particleCandidate.elem(aboveUpperBound, Op.EQUAL, getOptimisationProblem().getUpperBounds().elem(aboveUpperBound));

    velocityCandidate.elem(belowLowerBound, Op.EQUAL, velocityCandidate.elem(belowLowerBound).times(-0.5));
    velocityCandidate.elem(aboveUpperBound, Op.EQUAL, velocityCandidate.elem(aboveUpperBound).times(-0.5));

    _velocities.set(_activeParticle, velocityCandidate);
    _particles.set(_activeParticle, particleCandidate);

    nextParameters.add(particleCandidate);
    return nextParameters;
  }

  @Override
  protected void updateInformations(List<Col> parameters, List<Double> objectiveValues, List<Boolean> isSatisfyingConstraints) {
    for(int n = 0; n < parameters.size(); n++) {
      if (isSatisfyingConstraints.get(n)) {
        if (objectiveValues.get(n) < _localBestObjectiveValue.get(_activeParticle)) {
          _localBestParameters.set(_activeParticle, parameters.get(n));
          _localBestObjectiveValue.set(_activeParticle, objectiveValues.get(n));
        }

        if (objectiveValues.get(n) < _globalBestObjectiveValue) {
          _globalBestParameter = parameters.get(n);
          _globalBestObjectiveValue = objectiveValues.get(n);
          _randomizeTopology = false;
        } else {
          _randomizeTopology = true;
        }
      }

      _activeParticle = ++_activeParticle % _populationSize;
    }
  }

}
