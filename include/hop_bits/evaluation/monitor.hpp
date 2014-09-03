#pragma once

#include <armadillo>

namespace hop {
  class Monitor {
    public:
      Monitor() = default;

      static void observeSolutionCandidates();
      static void addSolutionCandidate(const arma::Col<double>& solutionCandidate);
      static arma::Mat<double> getSolutionCandidates();

      static void observeObjectiveValues();
      static void addObjectiveValue(const double& objectiveValue);
      static arma::Row<double> getObjectiveValues();

      static void observeSoftConstraintsValues();
      static void addSoftConstraintsValue(const double& softConstraintsValue);
      static arma::Row<double> getSoftConstraintsValues();

      static void observeNumberOfEvaluations();
      static void addNumberOfEvaluations(const unsigned int& numberOfEvaluations);
      static arma::Row<arma::uword> getNumberOfEvaluations();

      static void observeNumberOfIterations();
      static void addNumberOfIterations(const unsigned int& numberOfIterations);
      static arma::Row<arma::uword> getNumberOfIterations();

    protected:
      static bool _observeSolutionCandidates;
      static arma::Mat<double> _solutionCandidates;

      static bool _observeoOjectiveValues;
      static arma::Row<double> _objectiveValues;

      static bool _observeSoftConstraintsValues;
      static arma::Row<double> _softConstraintsValues;

      static bool _observeNumberOfEvaluations;
      static arma::Row<arma::uword> _numberOfEvaluations;

      static bool _observeNumberOfIterations;
      static arma::Row<arma::uword> _numberOfIterations;
  };
}
