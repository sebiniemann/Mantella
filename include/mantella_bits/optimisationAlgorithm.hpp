#pragma once

// C++ standard library
#include <chrono>
#include <functional>
#include <string>
#include <utility>
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class OptimisationAlgorithm {
   public:
    explicit OptimisationAlgorithm();

    void optimise(
        OptimisationProblem& optimisationProblem,
        const arma::Mat<double>& initialParameters);

    void setNextParametersFunction(
        std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> nextParameterFunction,
        const std::string& nextParametersFunctionName);
    void setNextParametersFunction(
        std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> nextParameterFunction);
    std::string getNextParametersFunctionName() const;
    void setBoundariesHandlingFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters_, const arma::Mat<arma::uword>& isBelowLowerBound_, const arma::Mat<arma::uword>& isAboveUpperBound_)> boundariesHandlingFunction,
        const std::string& boundariesHandlingFunctionName);
    void setBoundariesHandlingFunction(
        std::function<arma::Mat<double>(const arma::Mat<double>& parameters_, const arma::Mat<arma::uword>& isBelowLowerBound_, const arma::Mat<arma::uword>& isAboveUpperBound_)> boundariesHandlingFunction);
    std::string getBoundariesHandlingFunctionName() const;
    void setIsStagnatingFunction(
        std::function<bool(const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> isStagnatingFunction,
        const std::string& isStagnatingFunctionName);
    void setIsStagnatingFunction(
        std::function<bool(const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> isStagnatingFunction);
    std::string getIsStagnatingFunctionName() const;
    void setRestartingFunction(
        std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> restartingFunction,
        const std::string& restartingFunctionName);
    void setRestartingFunction(
        std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> restartingFunction);
    std::string getRestartingFunctionName() const;

    void setAcceptableObjectiveValue(
        const double acceptableObjectiveValue);
    double getAcceptableObjectiveValue() const;
    void setMaximalNumberOfIterations(
        const arma::uword maximalNumberOfIterations);
    arma::uword getMaximalNumberOfIterations() const;
    void setMaximalDuration(
        const std::chrono::microseconds maximalDuration);
    std::chrono::microseconds getMaximalDuration() const;

    bool isFinished() const;
    bool isTerminated() const;

    arma::uword getNumberOfIterations() const;
    std::chrono::microseconds getDuration() const;

    double getBestObjectiveValue() const;
    arma::Col<double> getBestParameter() const;

    std::vector<std::pair<arma::Col<double>, double>> getRecordedSampling() const;

    void reset();

    virtual ~OptimisationAlgorithm() = default;

   protected:
    int nodeRank_;
    int numberOfNodes_;

    std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> nextParametersFunction_;
    std::string nextParametersFunctionName_;
    std::function<arma::Mat<double>(const arma::Mat<double>& parameters_, const arma::Mat<arma::uword>& isBelowLowerBound_, const arma::Mat<arma::uword>& isAboveUpperBound_)> boundariesHandlingFunction_;
    std::string boundariesHandlingFunctionName_;
    std::function<bool(const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> isStagnatingFunction_;
    std::string isStagnatingFunctionName_;
    std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> restartingFunction_;
    std::string restartingFunctionName_;

    double acceptableObjectiveValue_;

    arma::uword maximalNumberOfIterations_;
    arma::uword numberOfIterations_;
    std::chrono::microseconds maximalDuration_;
    std::chrono::microseconds duration_;
    std::chrono::time_point<std::chrono::steady_clock> initialTimePoint_;

    double bestObjectiveValue_;
    arma::Col<double> bestParameter_;

    std::vector<std::pair<arma::Col<double>, double>> recordedSampling_;

    virtual void initialise(
        const arma::uword numberOfDimensions,
        const arma::Mat<double>& initialParameters);

    std::pair<arma::Row<double>, arma::Row<double>> evaluate(
        OptimisationProblem& optimisationProblem,
        const arma::Mat<double>& parameters);
  };
}
