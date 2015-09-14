#pragma once

// C++ standard library
#include <vector>
#include <utility>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/printable.hpp>
#include <mantella_bits/optimisationProblem.hpp>

namespace mant {
  class OptimisationAlgorithm : public Printable {
    public:
      explicit OptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      void optimise();

      void setMaximalNumberOfIterations(
          const arma::uword maximalNumberOfIterations);
          
      arma::uword getNumberOfIterations() const;
      arma::uword getMaximalNumberOfIterations() const;

      double getBestObjectiveValue() const;
      arma::Col<double> getBestParameter() const;
      
      bool isFinished() const;
      virtual bool isTerminated() const;

      std::vector<std::pair<arma::Col<double>, double>> getSamplingHistory() const;
      
      virtual ~OptimisationAlgorithm() = default;

    private:
      std::shared_ptr<OptimisationProblem> optimisationProblem_;

    protected:
      const arma::uword numberOfDimensions_;

      arma::uword numberOfIterations_;
      arma::uword maximalNumberOfIterations_;

      double bestObjectiveValue_;
      arma::Col<double> bestParameter_;

      std::vector<std::pair<arma::Col<double>, double>> samplingHistory_;

      int nodeRank_;
      int numberOfNodes_;

      arma::Col<double> getLowerBounds() const;
      arma::Col<double> getUpperBounds() const;

      arma::Col<arma::uword> isWithinLowerBounds(
          const arma::Col<double>& parameter) const;
      arma::Col<arma::uword> isWithinUpperBounds(
          const arma::Col<double>& parameter) const;

      double getAcceptableObjectiveValue() const;

      double getObjectiveValue(
          const arma::Col<double>& parameter);
      
      arma::Col<double> getRandomParameter() const;
      virtual arma::Col<double> getRandomNeighbour(
          const arma::Col<double>& parameter,
          const arma::Col<double>& minimalDistance,
          const arma::Col<double>& maximalDistance);
          
      bool updateBestParameter(
          const arma::Col<double>& parameter,
          const double objectiveValue);

      virtual void optimiseImplementation() = 0;
  };
}
