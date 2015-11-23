#pragma once

// C++ standard library
#include <vector>
#include <utility>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/helper/printable.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class OptimisationAlgorithm : public Printable {
   public:
    explicit OptimisationAlgorithm();

    void optimise(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setMaximalNumberOfIterations(
        const arma::uword maximalNumberOfIterations);
    arma::uword getNumberOfIterations() const;
    arma::uword getMaximalNumberOfIterations() const;

    void setMaximalDuration(
        const double duration);
    arma::uword getDuration() const;
    arma::uword getMaximalDuration() const;

    double getBestObjectiveValue() const;
    arma::Col<double> getBestParameter() const;

    bool isFinished() const;
    virtual bool isTerminated() const;

    void continue() const;
    void restart() const;

    std::vector<std::pair<arma::Col<double>, double>> getSamplingHistory() const;

   protected:
    int nodeRank_;
    int numberOfNodes_;

    arma::uword numberOfIterations_;
    arma::uword maximalNumberOfIterations_;
    arma::uword maximalDuration_;

    double bestObjectiveValue_;
    arma::Col<double> bestParameter_;

    std::vector<std::pair<arma::Col<double>, double>> samplingHistory_;
  };
}
