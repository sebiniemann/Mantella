#pragma once

#include <armadillo>

namespace hop {
  class Monitor {
    public:
      Monitor() = delete;
      Monitor(const Monitor&) = delete;
      Monitor& operator=(const Monitor&) = delete;

      static void observeCandidateSolutions();
      static void addCandidateSolution(const arma::Col<double>& candidateSolution);
      static arma::Mat<double> getCandidateSolutions();

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
      static bool observeCandidateSolutions_;
      static arma::Mat<double> candidateSolutions_;

      static bool observeoOjectiveValues_;
      static arma::Row<double> objectiveValues_;

      static bool observeSoftConstraintsValues_;
      static arma::Row<double> softConstraintsValues_;

      static bool observeNumberOfEvaluations_;
      static arma::Row<arma::uword> numberOfEvaluations_;

      static bool observeNumberOfIterations_;
      static arma::Row<arma::uword> numberOfIterations_;
  };
}
