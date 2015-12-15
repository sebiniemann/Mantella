#pragma once

// C++ standard library
#include <chrono>
#include <deque>
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
        OptimisationProblem& optimisationProblem,
        const arma::Mat<double>& initialParameters);
        
    void setNextParametersFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> nextParameterFunction,
        const std::string& nextParametersFunctionName);
    void setNextParametersFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> nextParameterFunction);
    std::string getNextParametersFunctionName() const;
    void setBoundariesHandlingFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters)> boundariesHandlingFunction,
        const std::string& boundariesHandlingFunctionName);
    void setBoundariesHandlingFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters)> boundariesHandlingFunction);
    std::string getBoundariesHandlingFunctionName() const;
    void setDegenerationDetectionFunction(
        std::function<bool(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationDetectionFunction,
        const std::string& degenerationDetectionFunctionName);
    void setDegenerationDetectionFunction(
        std::function<bool(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationDetectionFunction);
    std::string getDegenerationDetectionFunctionName() const;
    void setDegenerationHandlingFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationHandlingFunction,
        const std::string& degenerationHandlingFunctionName);
    void setDegenerationHandlingFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationHandlingFunction);
    std::string getDegenerationHandlingFunctionName() const;
        
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
   
    std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> nextParametersFunction_;
    std::string nextParametersFunctionName_;
    std::function<arma::Mat<double>(const arma::Mat<double>& parameters)> boundariesHandlingFunction_;
    std::string boundariesHandlingFunctionName_;
    std::function<bool(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationDetectionFunction_;
    std::string degenerationDetectionFunctionName_;
    std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationHandlingFunction_;
    std::string degenerationHandlingFunctionName_;

    double acceptableObjectiveValue_;

    arma::uword maximalNumberOfIterations_;
    arma::uword numberOfIterations_;
    std::chrono::microseconds maximalDuration_;
    std::chrono::microseconds duration_;
    std::chrono::time_point<std::chrono::steady_clock> initialTimePoint_;

    double bestObjectiveValue_;
    arma::Col<double> bestParameter_;

    std::vector<std::pair<arma::Col<double>, double>> samplingHistory_;
    
    void initialise(
        OptimisationProblem& optimisationProblem,
        const arma::Mat<double>& initialParameters);
    
    arma::Col<double> evaluate(
        OptimisationProblem& optimisationProblem,
        const arma::Mat<double>& parameters);
  };
}
