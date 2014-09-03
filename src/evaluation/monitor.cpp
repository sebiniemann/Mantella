#include <hop_bits/evaluation/monitor.hpp>

namespace hop {
  decltype(Monitor::_observeSolutionCandidates) Monitor::_observeSolutionCandidates = false;
  decltype(Monitor::_solutionCandidates) Monitor::_solutionCandidates;

  decltype(Monitor::_observeoOjectiveValues) Monitor::_observeoOjectiveValues = false;
  decltype(Monitor::_objectiveValues) Monitor::_objectiveValues;

  decltype(Monitor::_observeSoftConstraintsValues) Monitor::_observeSoftConstraintsValues = false;
  decltype(Monitor::_softConstraintsValues) Monitor::_softConstraintsValues;

  decltype(Monitor::_observeNumberOfEvaluations) Monitor::_observeNumberOfEvaluations = false;
  decltype(Monitor::_numberOfEvaluations) Monitor::_numberOfEvaluations;

  decltype(Monitor::_observeNumberOfIterations) Monitor::_observeNumberOfIterations = false;
  decltype(Monitor::_numberOfIterations) Monitor::_numberOfIterations;

  void Monitor::observeSolutionCandidates() {
    _observeSolutionCandidates = true;
  }

  void Monitor::addSolutionCandidate(const arma::Col<double>& solutionCandidate) {
    //TODO Check size

    if(_observeSolutionCandidates) {
      _solutionCandidates.insert_rows(_solutionCandidates.n_rows, solutionCandidate);
    }
  }

  arma::Mat<double> Monitor::getSolutionCandidates() {
    return _solutionCandidates;
  }

  void Monitor::observeObjectiveValues() {
    _objectiveValues = true;
  }

  void Monitor::addObjectiveValue(const double& objectiveValue) {
    if(_observeoOjectiveValues) {
      _objectiveValues.insert_rows(_objectiveValues.n_rows, arma::Col<double>({objectiveValue}));
    }
  }

  arma::Row<double> Monitor::getObjectiveValues() {
    return _objectiveValues;
  }

  void Monitor::observeSoftConstraintsValues() {
    _observeSoftConstraintsValues = true;
  }

  void Monitor::addSoftConstraintsValue(const double& softConstraintsValue) {
    if(_observeoOjectiveValues) {
      _softConstraintsValues.insert_rows(_softConstraintsValues.n_rows, arma::Col<double>({softConstraintsValue}));
    }
  }

  arma::Row<double> Monitor::getSoftConstraintsValues() {
    return _softConstraintsValues;
  }

  void Monitor::observeNumberOfEvaluations() {
    _observeNumberOfEvaluations = true;
  }

  void Monitor::addNumberOfEvaluations(const unsigned int& numberOfEvaluations) {
    if(_observeNumberOfEvaluations) {
      _numberOfEvaluations.insert_rows(_numberOfEvaluations.n_rows, arma::Col<arma::uword>({numberOfEvaluations}));
    }
  }

  arma::Row<arma::uword> Monitor::getNumberOfEvaluations() {
    return _numberOfIterations;
  }

  void Monitor::observeNumberOfIterations() {
    _observeNumberOfIterations = true;
  }

  void Monitor::addNumberOfIterations(const unsigned int& numberOfIterations) {
    if(_observeNumberOfIterations) {
      _numberOfIterations.insert_rows(_numberOfIterations.n_rows, arma::Col<arma::uword>({numberOfIterations}));
    }
  }

  arma::Row<arma::uword> Monitor::getNumberOfIterations() {
    return _numberOfIterations;
  }
}
