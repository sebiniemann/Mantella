#pragma once

// C++ standard library
#include <chrono>
#include <deque>
#include <memory>
#include <utility>
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  class OptimisationAlgorithm {
   public:
    explicit OptimisationAlgorithm();

    virtual void optimise(
        const std::shared_ptr<OptimisationProblem> optimisationProblem,
        const arma::Mat<double>& initialParameters);
        
    void setNextParametersFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> nextParameterFunction);
    void setBoundaryHandlingFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters)> boundaryHandlingFunction);
    void setIsDegeneratedFunction(
        std::function<bool(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> isDegeneratedFunction);
    void setDegenerationHandlingFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationHandlingFunction);
        
    void setAcceptableObjectiveValue(
        const double acceptableObjectiveValue);

    void setMaximalNumberOfIterations(
        const arma::uword maximalNumberOfIterations);
    arma::uword getNumberOfIterations() const;
    void setMaximalDuration(
        const std::chrono::microseconds maximalDuration);
    std::chrono::microseconds getDuration() const;

    bool isFinished() const;
    bool isTerminated() const;

    double getBestObjectiveValue() const;
    arma::Col<double> getBestParameter() const;

    std::vector<std::pair<arma::Col<double>, double>> getSamplingHistory() const;
    
    void reset();

   protected:
    int nodeRank_;
    int numberOfNodes_;
   
    std::function<arma::Col<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> nextParametersFunction_;
    std::function<arma::Mat<double>(const arma::Mat<double>& parameters)> boundaryHandlingFunction_;
    std::function<bool(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> isDegeneratedFunction_;
    std::function<arma::Col<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationHandlingFunction_;

    double acceptableObjectiveValue_;

    arma::uword maximalNumberOfIterations_;
    arma::uword numberOfIterations_;
    std::chrono::microseconds maximalDuration_;
    std::chrono::microseconds duration_;
    std::chrono::time_point<std::chrono::steady_clock> initialTimePoint_;

    double bestObjectiveValue_;
    arma::Col<double> bestParameter_;

    std::vector<std::pair<arma::Col<double>, double>> samplingHistory_;
    
    arma::Col<double> evaluate(
        const std::shared_ptr<OptimisationProblem> optimisationProblem,
        const arma::Mat<double>& parameters);
  };
}
