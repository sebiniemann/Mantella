#pragma once
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <chrono>
#include <functional>
#include <string>
#include <utility>
#include <vector>

// MPI
#if defined(SUPPORT_MPI)
#include <mpi.h> // IWYU pragma: keep
#endif

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

    void setInitialisingFunctions(
        const std::vector<std::pair<
            std::function<
                arma::mat(
                    const arma::uword numberOfDimensions_,
                    const arma::mat& initialParameters_)>,
            std::string>>& initialisingFunctions);

    std::vector<std::pair<std::function<arma::mat(const arma::uword numberOfDimensions_, const arma::mat& initialParameters_)>, std::string>> getInitialisingFunctions() const;

    void setNextParametersFunctions(
        const std::vector<std::pair<
            std::function<arma::mat(
                const arma::uword numberOfDimensions_,
                const arma::mat& parameters_,
                const arma::rowvec& objectiveValues_,
                const arma::rowvec& differences_)>,
            std::string>>& nextParametersFunctions);

    std::vector<std::pair<std::function<arma::mat(const arma::uword numberOfDimensions_, const arma::mat& parameters_, const arma::rowvec& objectiveValues_, const arma::rowvec& differences_)>, std::string>> getNextParametersFunctions() const;

    void setBoundariesHandlingFunctions(
        const std::vector<std::pair<
            std::function<arma::mat(
                const arma::mat& parameters_,
                const arma::umat& isBelowLowerBound_,
                const arma::umat& isAboveUpperBound_)>,
            std::string>>& boundariesHandlingFunctions);

    std::vector<std::pair<std::function<arma::mat(const arma::mat& parameters_, const arma::umat& isBelowLowerBound_, const arma::umat& isAboveUpperBound_)>, std::string>> getBoundariesHandlingFunctions() const;

    void setIsStagnatingFunctions(
        const std::vector<std::pair<
            std::function<bool(
                const arma::mat& parameters_,
                const arma::rowvec& objectiveValues_,
                const arma::rowvec& differences_)>,
            std::string>>& isStagnatingFunctions);

    std::vector<std::pair<std::function<bool(const arma::mat& parameters_, const arma::rowvec& objectiveValues_, const arma::rowvec& differences_)>, std::string>> getIsStagnatingFunctions() const;

    void setRestartingFunctions(
        const std::vector<std::pair<
            std::function<arma::mat(
                const arma::uword numberOfDimensions_,
                const arma::mat& parameters_,
                const arma::rowvec& objectiveValues_,
                const arma::rowvec& differences_)>,
            std::string>>& restartingFunctions);

    std::vector<std::pair<std::function<arma::mat(const arma::uword numberOfDimensions_, const arma::mat& parameters_, const arma::rowvec& objectiveValues_, const arma::rowvec& differences_)>, std::string>> getRestartingFunctions() const;

    void setCommunicationFunctions(
        const std::vector<std::pair<
            std::function<arma::mat(
                const arma::uword numberOfDimensions_,
                const arma::mat& parameters_,
                const arma::rowvec& objectiveValues_,
                const arma::rowvec& differences_)>,
            std::string>>& communicationFunctions);

    std::vector<std::pair<std::function<arma::mat(const arma::uword numberOfDimensions_, const arma::mat& parameters_, const arma::rowvec& objectiveValues_, const arma::rowvec& differences_)>, std::string>> getCommunicationFunctions() const;

    void optimise(
        OptimisationProblem& optimisationProblem,
        const arma::mat& initialParameters);

    void setNumberOfCommunicationStalls(
        const arma::uword numberOfCommunicationStalls);
    arma::uword getNumberOfCommunicationStalls() const;

    void setAcceptableObjectiveValue(
        const double acceptableObjectiveValue);
    double getAcceptableObjectiveValue() const;

    void setMaximalNumberOfIterations(
        const arma::uword maximalNumberOfIterations);
    arma::uword getMaximalNumberOfIterations() const;

    void setMaximalDuration(
        const std::chrono::microseconds maximalDuration);
    std::chrono::microseconds getMaximalDuration() const;

    arma::uword getUsedNumberOfIterations() const;
    std::chrono::microseconds getUsedDuration() const;

    bool isFinished() const;
    bool isTerminated() const;

    arma::vec getBestFoundParameter() const;
    double getBestFoundObjectiveValue() const;

    std::vector<std::pair<arma::vec, double>> getRecordedSampling() const;

    void reset();

   protected:
    std::vector<std::pair<
        std::function<arma::mat(
            const arma::uword numberOfDimensions_,
            const arma::mat& initialParameters_)>,
        std::string>>
        initialisingFunctions_;

    std::vector<std::pair<
        std::function<arma::mat(
            const arma::uword numberOfDimensions_,
            const arma::mat& parameters_,
            const arma::rowvec& objectiveValues_,
            const arma::rowvec& differences_)>,
        std::string>>
        nextParametersFunctions_;

    std::vector<std::pair<
        std::function<arma::mat(
            const arma::mat& parameters_,
            const arma::umat& isBelowLowerBound_,
            const arma::umat& isAboveUpperBound_)>,
        std::string>>
        boundariesHandlingFunctions_;

    std::vector<std::pair<
        std::function<bool(
            const arma::mat& parameters_,
            const arma::rowvec& objectiveValues_,
            const arma::rowvec& differences_)>,
        std::string>>
        isStagnatingFunctions_;

    std::vector<std::pair<
        std::function<arma::mat(
            const arma::uword numberOfDimensions_,
            const arma::mat& parameters_,
            const arma::rowvec& objectiveValues_,
            const arma::rowvec& differences_)>,
        std::string>>
        restartingFunctions_;

    std::vector<std::pair<
        std::function<arma::mat(
            const arma::uword numberOfDimensions_,
            const arma::mat& parameters_,
            const arma::rowvec& objectiveValues_,
            const arma::rowvec& differences_)>,
        std::string>>
        communicationFunctions_;

    arma::uword numberOfCommunicationStalls_;

    double acceptableObjectiveValue_;
    arma::uword maximalNumberOfIterations_;
    std::chrono::microseconds maximalDuration_;

    arma::uword usedNumberOfIterations_;
    std::chrono::microseconds usedDuration_;

    double bestFoundObjectiveValue_;
    arma::vec bestFoundParameter_;

    std::vector<std::pair<arma::vec, double>> recordedSampling_;

    std::chrono::time_point<std::chrono::steady_clock> initialTimePoint_;

    std::pair<arma::rowvec, arma::rowvec> getObjectiveValuesAndDifferences(
        OptimisationProblem& optimisationProblem,
        const arma::mat& parameters);

    arma::mat normalisedParameters(
        const OptimisationProblem& optimisationProblem,
        const arma::mat& parameters) const;
    arma::mat denormalisedParameters(
        const OptimisationProblem& optimisationProblem,
        const arma::mat& normalisedParameters) const;

#if defined(SUPPORT_MPI)
    // Declares a custom MPI_User_function (used as custom MPI_Reduce* operation).
    // **Note:** This function will not be available without MPI support, as `MPI_Datatype` is an MPI specific type and needs to be defined.
    // @see https://www.mpi-forum.org/docs/ for more information.
    static void mpiOpBestSample(
        void* firstInput,
        void* secondInput,
        int* size,
        MPI_Datatype* type);
#endif
  };
}
