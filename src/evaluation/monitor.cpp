#include <hop_bits/evaluation/monitor.hpp>

namespace hop {
  decltype(Monitor::observeCandidateSolutions_) Monitor::observeCandidateSolutions_ = false;
  decltype(Monitor::candidateSolutions_) Monitor::candidateSolutions_;

  decltype(Monitor::observeoOjectiveValues_) Monitor::observeoOjectiveValues_ = false;
  decltype(Monitor::objectiveValues_) Monitor::objectiveValues_;

  decltype(Monitor::observeSoftConstraintsValues_) Monitor::observeSoftConstraintsValues_ = false;
  decltype(Monitor::softConstraintsValues_) Monitor::softConstraintsValues_;

  decltype(Monitor::observeNumberOfEvaluations_) Monitor::observeNumberOfEvaluations_ = false;
  decltype(Monitor::numberOfEvaluations_) Monitor::numberOfEvaluations_;

  decltype(Monitor::observeNumberOfIterations_) Monitor::observeNumberOfIterations_ = false;
  decltype(Monitor::numberOfIterations_) Monitor::numberOfIterations_;

  void Monitor::observeCandidateSolutions() {
    observeCandidateSolutions_ = true;
  }

  void Monitor::addCandidateSolution(const arma::Col<double>& candidateSolution) {
    //TODO Check size

    if(observeCandidateSolutions_) {
      candidateSolutions_.insert_rows(candidateSolutions_.n_rows, candidateSolution);
    }
  }

  arma::Mat<double> Monitor::getCandidateSolutions() {
    return candidateSolutions_;
  }

  void Monitor::observeObjectiveValues() {
    objectiveValues_ = true;
  }

  void Monitor::addObjectiveValue(const double& objectiveValue) {
    if(observeoOjectiveValues_) {
      objectiveValues_.insert_rows(objectiveValues_.n_rows, arma::Col<double>({objectiveValue}));
    }
  }

  arma::Row<double> Monitor::getObjectiveValues() {
    return objectiveValues_;
  }

  void Monitor::observeSoftConstraintsValues() {
    observeSoftConstraintsValues_ = true;
  }

  void Monitor::addSoftConstraintsValue(const double& softConstraintsValue) {
    if(observeoOjectiveValues_) {
      softConstraintsValues_.insert_rows(softConstraintsValues_.n_rows, arma::Col<double>({softConstraintsValue}));
    }
  }

  arma::Row<double> Monitor::getSoftConstraintsValues() {
    return softConstraintsValues_;
  }

  void Monitor::observeNumberOfEvaluations() {
    observeNumberOfEvaluations_ = true;
  }

  void Monitor::addNumberOfEvaluations(const unsigned int& numberOfEvaluations) {
    if(observeNumberOfEvaluations_) {
      numberOfEvaluations_.insert_rows(numberOfEvaluations_.n_rows, arma::Col<arma::uword>({numberOfEvaluations}));
    }
  }

  arma::Row<arma::uword> Monitor::getNumberOfEvaluations() {
    return numberOfIterations_;
  }

  void Monitor::observeNumberOfIterations() {
    observeNumberOfIterations_ = true;
  }

  void Monitor::addNumberOfIterations(const unsigned int& numberOfIterations) {
    if(observeNumberOfIterations_) {
      numberOfIterations_.insert_rows(numberOfIterations_.n_rows, arma::Col<arma::uword>({numberOfIterations}));
    }
  }

  arma::Row<arma::uword> Monitor::getNumberOfIterations() {
    return numberOfIterations_;
  }
}
