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
#if defined(MANTELLA_USE_MPI)
  void mpiGetBestParameter(
      void* firstInput,
      void* secondInput,
      int* size,
      MPI_Datatype* type);
#endif

  class OptimisationAlgorithm : public Printable {
    public:
      explicit OptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      void optimise();

      void setMaximalNumberOfIterations(
          const arma::uword maximalNumberOfIterations);
          
      arma::uword getNumberOfIterations() const;
      arma::uword getMaximalNumberOfIterations() const;

      arma::Col<double> getBestParameter() const;
      double getBestSoftConstraintsValue() const;
      double getBestObjectiveValue() const;
      
      bool isFinished() const;
      virtual bool isTerminated() const;

      std::vector<std::pair<arma::Col<double>, double>> getSamplingProgress() const;
      
      virtual ~OptimisationAlgorithm() = default;

    private:
      std::shared_ptr<OptimisationProblem> optimisationProblem_;

    protected:
      const arma::uword numberOfDimensions_;

      arma::uword numberOfIterations_;
      arma::uword maximalNumberOfIterations_;

      double bestSoftConstraintsValue_;
      double bestObjectiveValue_;
      arma::Col<double> bestParameter_;

      std::vector<std::pair<arma::Col<double>, double>> samplingProgress_;

      int nodeRank_;
      int numberOfNodes_;

      arma::Col<double> getLowerBounds() const;
      arma::Col<double> getUpperBounds() const;

      arma::Col<arma::uword> isWithinLowerBounds(
          const arma::Col<double>& parameter) const;
      arma::Col<arma::uword> isWithinUpperBounds(
          const arma::Col<double>& parameter) const;
      bool isSatisfyingSoftConstraints(
          const arma::Col<double>& parameter) const;
      bool isSatisfyingConstraints(
          const arma::Col<double>& parameter) const;

      double getAcceptableObjectiveValue() const;

      double getSoftConstraintsValue(
          const arma::Col<double>& parameter) const;
      double getObjectiveValue(
          const arma::Col<double>& parameter);
      
      arma::Col<double> getRandomParameter() const;
      arma::Col<double> getRandomNeighbour(
          const arma::Col<double>& parameter,
          const double minimalDistance,
          const double maximalDistance) const;
          
      bool updateBestParameter(
          const arma::Col<double>& parameter,
          const double softConstraintsValue,
          const double objectiveValue);

      virtual void optimiseImplementation() = 0;
  };
}