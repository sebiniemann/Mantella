#include "mantella_bits/optimisationAlgorithm.hpp"

// C++ standard library
#if defined(SUPPORT_MPI)
#include <algorithm> // IWYU pragma: keep
#endif
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#if defined(SUPPORT_MPI)
#include <stdexcept> // IWYU pragma: keep
#endif

// MPI
// IWYU pragma: no_include "mpi.h"

// Mantella
#if defined(SUPPORT_MPI)
#include "mantella_bits/assert.hpp" // IWYU pragma: keep
#endif
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  OptimisationAlgorithm::OptimisationAlgorithm() {
    reset();

    setNumberOfCommunicationStalls(0);
    setAcceptableObjectiveValue(-std::numeric_limits<double>::infinity());
    setMaximalNumberOfIterations(std::numeric_limits<arma::uword>::max());
    setMaximalDuration(std::chrono::seconds(1));

    setBoundariesHandlingFunctions(
        {{[this](
              const arma::mat& parameters_,
              const arma::umat& isBelowLowerBound_,
              const arma::umat& isAboveUpperBound_) {
            return arma::clamp(parameters_, 0.0, 1.0);
          },
          "Set parts of an parameter that are out of bound directly to to bound"}});

    setRestartingFunctions(
        {{[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_) {
            return uniformRandomNumbers(parameters_.n_rows, parameters_.n_cols);
          },
          "Place all parameters randomly within the bounds"}});

    setCommunicationFunctions(
        {{[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_) { return parameters_; },
          "Find the cluster-wide best found sample"}});
  }

  void OptimisationAlgorithm::setInitialisingFunctions(
      const std::vector<std::pair<
          std::function<arma::mat(
              const arma::uword numberOfDimensions_,
              const arma::mat& initialParameters_)>,
          std::string>>& initialisingFunctions) {
    for (const auto& initialisingFunction : initialisingFunctions) {
      assert(static_cast<bool>(initialisingFunction.first) && "OptimisationAlgorithm.setInitialisingFunctions: All initialising functions must be callable.");
    }

    initialisingFunctions_ = initialisingFunctions;
  }

  std::vector<std::pair<std::function<arma::mat(const arma::uword numberOfDimensions_, const arma::mat& initialParameters_)>, std::string>> OptimisationAlgorithm::getInitialisingFunctions() const {
    return initialisingFunctions_;
  }

  void OptimisationAlgorithm::setNextParametersFunctions(
      const std::vector<std::pair<
          std::function<arma::mat(
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_)>,
          std::string>>& nextParametersFunctions) {
    assert(!nextParametersFunctions.empty() && "OptimisationAlgorithm.setNextParametersFunctions: At least one next parameters function must be defined.");
    for (const auto& nextParametersFunction : nextParametersFunctions) {
      assert(static_cast<bool>(nextParametersFunction.first) && "OptimisationAlgorithm.setNextParametersFunctions: All *next parameters* functions must be callable.");
    }

    nextParametersFunctions_ = nextParametersFunctions;
  }

  std::vector<std::pair<std::function<arma::mat(const arma::uword numberOfDimensions_, const arma::mat& parameters_, const arma::rowvec& objectiveValues_, const arma::rowvec& differences_)>, std::string>> OptimisationAlgorithm::getNextParametersFunctions() const {
    return nextParametersFunctions_;
  }

  void OptimisationAlgorithm::setBoundariesHandlingFunctions(
      const std::vector<std::pair<
          std::function<arma::mat(
              const arma::mat& parameters_,
              const arma::umat& isBelowLowerBound_,
              const arma::umat& isAboveUpperBound_)>,
          std::string>>& boundariesHandlingFunctions) {
    for (const auto& boundariesHandlingFunction : boundariesHandlingFunctions) {
      assert(static_cast<bool>(boundariesHandlingFunction.first) && "OptimisationAlgorithm.setBoundariesHandlingFunctions: All boundaries-handling functions must be callable.");
    }

    boundariesHandlingFunctions_ = boundariesHandlingFunctions;
  }

  std::vector<std::pair<std::function<arma::mat(const arma::mat& parameters_, const arma::umat& isBelowLowerBound_, const arma::umat& isAboveUpperBound_)>, std::string>> OptimisationAlgorithm::getBoundariesHandlingFunctions() const {
    return boundariesHandlingFunctions_;
  }

  void OptimisationAlgorithm::setIsStagnatingFunctions(
      const std::vector<std::pair<
          std::function<bool(
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_)>,
          std::string>>& isStagnatingFunctions) {
    for (const auto& isStagnatingFunction : isStagnatingFunctions) {
      assert(static_cast<bool>(isStagnatingFunction.first) && "OptimisationAlgorithm.setIsStagnatingFunctions: All *is stagnating* functions must be callable.");
    }

    isStagnatingFunctions_ = isStagnatingFunctions;
  }

  std::vector<std::pair<std::function<bool(const arma::mat& parameters_, const arma::rowvec& objectiveValues_, const arma::rowvec& differences_)>, std::string>> OptimisationAlgorithm::getIsStagnatingFunctions() const {
    return isStagnatingFunctions_;
  }

  void OptimisationAlgorithm::setRestartingFunctions(
      const std::vector<std::pair<
          std::function<arma::mat(
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_)>,
          std::string>>& restartingFunctions) {
    for (const auto& restartingFunction : restartingFunctions) {
      assert(static_cast<bool>(restartingFunction.first) && "OptimisationAlgorithm.setRestartingFunctions: All restarting functions must be callable.");
    }

    restartingFunctions_ = restartingFunctions;
  }

  std::vector<std::pair<std::function<arma::mat(const arma::uword numberOfDimensions_, const arma::mat& parameters_, const arma::rowvec& objectiveValues_, const arma::rowvec& differences_)>, std::string>> OptimisationAlgorithm::getRestartingFunctions() const {
    return restartingFunctions_;
  }

  void OptimisationAlgorithm::setCommunicationFunctions(
      const std::vector<std::pair<
          std::function<arma::mat(
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_)>,
          std::string>>& communicationFunctions) {
    for (const auto& restartingFunction : communicationFunctions) {
      assert(static_cast<bool>(restartingFunction.first) && "OptimisationAlgorithm.setCommunicationFunctions: All communication functions must be callable.");
    }

    communicationFunctions_ = communicationFunctions;
  }

  std::vector<std::pair<std::function<arma::mat(const arma::uword numberOfDimensions_, const arma::mat& parameters_, const arma::rowvec& objectiveValues_, const arma::rowvec& differences_)>, std::string>> OptimisationAlgorithm::getCommunicationFunctions() const {
    return communicationFunctions_;
  }

  void OptimisationAlgorithm::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::mat& initialParameters) {
    assert(!initialParameters.is_empty() && "OptimisationAlgorithm.optimise: The initial parameter's number of columns must be greater than 0.");
    assert(initialParameters.n_rows == optimisationProblem.numberOfDimensions_ && "OptimisationAlgorithm.optimise: The initial parameter's number of rows must be equal to the optimisation problem's number of dimensions.");
    assert(!nextParametersFunctions_.empty() && "OptimisationAlgorithm.optimise: At least one next parameters function must be defined.");

#if defined(SUPPORT_MPI)
    if (optimisationProblem.numberOfDimensions_ > std::numeric_limits<int>::max() - 2) {
      throw std::range_error("OptimisationAlgorithm.optimise: The number of dimensions must be less than the largest supported integer minus 2 if used with MPI support.");
    } else if (!isRepresentableAsFloatingPoint(optimisationProblem.numberOfDimensions_)) {
      throw std::range_error("OptimisationAlgorithm.optimise: The number of dimensions must be representable as a floating point if used with MPI support.");
    }
#endif

    if (::mant::isVerbose) {
      const auto functionNameConcatenatingFunction = [](
          const std::string& accumulatedName,
          const auto& nextFunction) {
        return accumulatedName + "\n  - " + nextFunction.second;
      };

      const auto& objectiveFunctions = optimisationProblem.getObjectiveFunctions();

      std::cout << "================================================================================\n"
                << "Solving optimisation problem: "
                << std::accumulate(objectiveFunctions.begin() + 1, objectiveFunctions.end(), objectiveFunctions.at(0).second, functionNameConcatenatingFunction)
                << "\n"
                << "  Number of dimensions: " << optimisationProblem.numberOfDimensions_ << "\n"
                << "  Lower bounds: " << optimisationProblem.getLowerBounds().t()
                << "  Upper bounds: " << optimisationProblem.getUpperBounds().t()
                << "  Acceptable objective value: " << acceptableObjectiveValue_ << "\n"
                << "--------------------------------------------------------------------------------\n";

      std::cout << "  Initialising functions: \n  - "
                << (initialisingFunctions_.size() < 1 ? "Do nothing" : std::accumulate(initialisingFunctions_.begin() + 1, initialisingFunctions_.end(), initialisingFunctions_.at(0).second, functionNameConcatenatingFunction))
                << "\n";

      std::cout << "  Optimisation strategies: \n  - "
                << std::accumulate(nextParametersFunctions_.begin() + 1, nextParametersFunctions_.end(), nextParametersFunctions_.at(0).second, functionNameConcatenatingFunction)
                << "\n";

      std::cout << "  Boundaries-handling functions: \n  - "
                << (boundariesHandlingFunctions_.size() < 1 ? "Do nothing" : std::accumulate(boundariesHandlingFunctions_.begin() + 1, boundariesHandlingFunctions_.end(), boundariesHandlingFunctions_.at(0).second, functionNameConcatenatingFunction))
                << "\n";

      std::cout << "  Is-stagnating functions: \n  - "
                << (isStagnatingFunctions_.size() < 1 ? "Return always false" : std::accumulate(isStagnatingFunctions_.begin() + 1, isStagnatingFunctions_.end(), isStagnatingFunctions_.at(0).second, functionNameConcatenatingFunction))
                << "\n";

      std::cout << "  Restarting functions: \n  - "
                << (restartingFunctions_.size() < 1 ? "Do nothing" : std::accumulate(restartingFunctions_.begin() + 1, restartingFunctions_.end(), restartingFunctions_.at(0).second, functionNameConcatenatingFunction))
                << "\n";

#if defined(SUPPORT_MPI)
      std::cout << "  Communication functions: \n  - "
                << (communicationFunctions_.size() < 1 ? "Do nothing" : std::accumulate(communicationFunctions_.begin() + 1, communicationFunctions_.end(), communicationFunctions_.at(0).second, functionNameConcatenatingFunction))
                << "\n";
#endif

      std::cout << std::endl;
    }

    // **Note:** It is important to reset the function after the verbose output above, as we would otherwise add the time to print the output to the console to our computation time.
    reset();

    // Normalises the parameter such that (0, ..., 0) maps to the lower bound and (1, ..., 1) to the upper one.
    arma::mat parameters = normalisedParameters(optimisationProblem, initialParameters);
    for (const auto& initialisingFunction : initialisingFunctions_) {
      parameters = initialisingFunction.first(optimisationProblem.numberOfDimensions_, parameters);
      assert(!parameters.is_empty());
      assert(!parameters.has_nan());
      assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);
    }

    for (const auto& boundariesHandlingFunction : boundariesHandlingFunctions_) {
      parameters = boundariesHandlingFunction.first(parameters, parameters<0.0, parameters> 1.0);
      assert(!parameters.is_empty());
      assert(!parameters.has_nan());
      assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);
    }

    std::pair<arma::rowvec, arma::rowvec> objectiveValuesAndDifferences = getObjectiveValuesAndDifferences(optimisationProblem, parameters);

    while (!isTerminated() && !isFinished()) {
      for (arma::uword n = 0; n <= numberOfCommunicationStalls_; ++n) {
        bool isStagnating = false;
        for (const auto& isStagnatingFunction : isStagnatingFunctions_) {
          isStagnating = isStagnatingFunction.first(parameters, objectiveValuesAndDifferences.first, objectiveValuesAndDifferences.second);

          if (isStagnating) {
            break;
          }
        }

        if (isStagnating) {
          for (const auto& restartingFunction : restartingFunctions_) {
            parameters = restartingFunction.first(optimisationProblem.numberOfDimensions_, parameters, objectiveValuesAndDifferences.first, objectiveValuesAndDifferences.second);
            assert(!parameters.is_empty());
            assert(!parameters.has_nan());
            assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);
          }
        } else {
          for (const auto& nextParametersFunction : nextParametersFunctions_) {
            parameters = nextParametersFunction.first(optimisationProblem.numberOfDimensions_, parameters, objectiveValuesAndDifferences.first, objectiveValuesAndDifferences.second);
            assert(!parameters.is_empty());
            assert(!parameters.has_nan());
            assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);
          }
        }

        for (const auto& boundariesHandlingFunction : boundariesHandlingFunctions_) {
          parameters = boundariesHandlingFunction.first(parameters, parameters<0.0, parameters> 1.0);
          assert(!parameters.is_empty());
          assert(!parameters.has_nan());
          assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);
        }

        objectiveValuesAndDifferences = getObjectiveValuesAndDifferences(optimisationProblem, parameters);
        ++usedNumberOfIterations_;
      }

#if defined(SUPPORT_MPI)
      for (const auto& communicationFunction : communicationFunctions_) {
        parameters = communicationFunction.first(optimisationProblem.numberOfDimensions_, parameters, objectiveValuesAndDifferences.first, objectiveValuesAndDifferences.second);
        assert(!parameters.is_empty());
        assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);
      }
#endif
    }

#if defined(SUPPORT_MPI)
    MPI_Datatype MANT_MPI_SAMPLE;
    MPI_Type_contiguous(static_cast<int>(2 + optimisationProblem.numberOfDimensions_), MPI_DOUBLE, &MANT_MPI_SAMPLE);
    MPI_Type_commit(&MANT_MPI_SAMPLE);

    arma::vec mpiSendSample(2 + optimisationProblem.numberOfDimensions_);
    mpiSendSample(0) = static_cast<double>(optimisationProblem.numberOfDimensions_);
    mpiSendSample(1) = bestFoundObjectiveValue_;
    mpiSendSample.tail(optimisationProblem.numberOfDimensions_) = bestFoundParameter_;

    MPI_Op MPI_Op_best_sample;
    MPI_Op_create(&mpiOpBestSample, 1, &MPI_Op_best_sample);
    arma::vec mpiReceiveSample(arma::size(mpiSendSample));
    MPI_Allreduce(mpiSendSample.memptr(), mpiReceiveSample.memptr(), 1, MANT_MPI_SAMPLE, MPI_Op_best_sample, MPI_COMM_WORLD);

    bestFoundObjectiveValue_ = mpiReceiveSample(1);
    bestFoundParameter_ = mpiReceiveSample.tail(optimisationProblem.numberOfDimensions_);
#endif

    usedDuration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialTimePoint_);

    // Denormalises the best parameter, as until now, all parameters where calculated on a normalised parameter space.
    bestFoundParameter_ = denormalisedParameters(optimisationProblem, bestFoundParameter_);

    if (::mant::isVerbose) {
      if (isFinished()) {
        std::cout << "  Finished with an acceptable solution.\n";
      } else if (isTerminated()) {
        std::cout << "  Terminated (run out of time or iterations).\n";
      }

      std::cout << "    Took " << usedDuration_.count() << " / " << maximalDuration_.count() << " microseconds\n"
                << "    Took " << usedNumberOfIterations_ << " / " << maximalNumberOfIterations_ << " iterations\n"
                << "    Difference to the acceptable objective value: " << bestFoundObjectiveValue_ - acceptableObjectiveValue_ << "\n"
                << "    Best found objective value: " << bestFoundObjectiveValue_ << "\n"
                << "    Best found parameter: " << bestFoundParameter_.t()
                << std::endl;
    }
  }

  void OptimisationAlgorithm::setNumberOfCommunicationStalls(
      const arma::uword numberOfCommunicationStalls) {
    numberOfCommunicationStalls_ = numberOfCommunicationStalls;
  }

  arma::uword OptimisationAlgorithm::getNumberOfCommunicationStalls() const {
    return numberOfCommunicationStalls_;
  }

  void OptimisationAlgorithm::setAcceptableObjectiveValue(
      const double acceptableObjectiveValue) {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
  }

  double OptimisationAlgorithm::getAcceptableObjectiveValue() const {
    return acceptableObjectiveValue_;
  }

  void OptimisationAlgorithm::setMaximalNumberOfIterations(
      const arma::uword maximalNumberOfIterations) {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  arma::uword OptimisationAlgorithm::getMaximalNumberOfIterations() const {
    return maximalNumberOfIterations_;
  }

  void OptimisationAlgorithm::setMaximalDuration(
      const std::chrono::microseconds maximalDuration) {
    maximalDuration_ = maximalDuration;
  }

  std::chrono::microseconds OptimisationAlgorithm::getMaximalDuration() const {
    return maximalDuration_;
  }

  arma::uword OptimisationAlgorithm::getUsedNumberOfIterations() const {
    return usedNumberOfIterations_;
  }

  std::chrono::microseconds OptimisationAlgorithm::getUsedDuration() const {
    return usedDuration_;
  }

  bool OptimisationAlgorithm::isFinished() const {
    return bestFoundObjectiveValue_ <= acceptableObjectiveValue_;
  }

  bool OptimisationAlgorithm::isTerminated() const {
#if defined(SUPPORT_MPI)
    return usedNumberOfIterations_ >= maximalNumberOfIterations_;
#else
    return usedNumberOfIterations_ >= maximalNumberOfIterations_ || usedDuration_ >= maximalDuration_;
#endif
  }

  arma::vec OptimisationAlgorithm::getBestFoundParameter() const {
    return bestFoundParameter_;
  }

  double OptimisationAlgorithm::getBestFoundObjectiveValue() const {
    return bestFoundObjectiveValue_;
  }

  std::vector<std::pair<arma::vec, double>> OptimisationAlgorithm::getRecordedSampling() const {
    return recordedSampling_;
  }

  void OptimisationAlgorithm::reset() {
    usedNumberOfIterations_ = 0;
    bestFoundObjectiveValue_ = std::numeric_limits<double>::infinity();
    bestFoundParameter_.reset();
    usedDuration_ = std::chrono::microseconds(0);
    initialTimePoint_ = std::chrono::steady_clock::now();

    recordedSampling_.clear();
  }

  std::pair<arma::rowvec, arma::rowvec> OptimisationAlgorithm::getObjectiveValuesAndDifferences(
      OptimisationProblem& optimisationProblem, const arma::mat& parameters) {
    // Objects like `optimisationProblem` perform all validations by themselves.
    assert(!parameters.is_empty());
    assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);

    const double previousBestObjectiveValue = bestFoundObjectiveValue_;

    arma::rowvec objectiveValues(parameters.n_cols);
    arma::rowvec differences(parameters.n_cols);

    // **Note:** While the termination criteria for the maximal number of iterations and especially the maximal duration might be reached after each evaluation, we only need to check for an acceptable objective value if we found a better one.
    for (arma::uword n = 0; n < parameters.n_cols && !isTerminated(); ++n) {
      const arma::vec& parameter = parameters.col(n);
      const double objectiveValue = optimisationProblem.getObjectiveValueOfNormalisedParameter(parameter);

      if (::mant::isRecordingSampling) {
        recordedSampling_.push_back({denormalisedParameters(optimisationProblem, parameter), objectiveValue});
      }

      objectiveValues(n) = objectiveValue;
      differences(n) = objectiveValue - previousBestObjectiveValue;

      usedDuration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialTimePoint_);

      if (objectiveValue < bestFoundObjectiveValue_) {
        if (::mant::isVerbose) {
          std::cout << "  Iteration #" << usedNumberOfIterations_ << " (after "
                    << usedDuration_.count() << "ms)"
#if defined(SUPPORT_MPI)
                    << " On node " << nodeRank() << ""
#endif
                    << ": Found better solution.\n"
                    << "    Difference to the previous best objective value: " << objectiveValue - bestFoundObjectiveValue_ << "\n"
                    << "    Best found objective value: " << objectiveValue << "\n"
                    << "    Best found parameter: " << denormalisedParameters(optimisationProblem, parameter).t()
                    << std::endl;
        }

        bestFoundParameter_ = parameter;
        bestFoundObjectiveValue_ = objectiveValue;

        // Stops with further function evaluations if we found an optimal objective value.
        if (isFinished()) {
          break;
        }
      }
    }

    return {objectiveValues, differences};
  }

  arma::mat OptimisationAlgorithm::normalisedParameters(
      const OptimisationProblem& optimisationProblem,
      const arma::mat& parameters) const {
    assert(arma::all(optimisationProblem.getLowerBounds() <= optimisationProblem.getUpperBounds()));
    assert(!parameters.has_nan());

    arma::mat normalisedParameters(arma::size(parameters));

    for (arma::uword n = 0; n < normalisedParameters.n_rows; ++n) {
      if (std::abs(optimisationProblem.getLowerBounds()(n) - optimisationProblem.getUpperBounds()(n)) < ::mant::machinePrecision) {
        normalisedParameters.row(n).fill(optimisationProblem.getLowerBounds()(n));
      } else {
        normalisedParameters.row(n) = (parameters.row(n) - optimisationProblem.getLowerBounds()(n)) / (optimisationProblem.getUpperBounds()(n) - optimisationProblem.getLowerBounds()(n));
      }
    }

    return normalisedParameters;
  }

  arma::mat OptimisationAlgorithm::denormalisedParameters(
      const OptimisationProblem& optimisationProblem,
      const arma::mat& normalisedParameters) const {
    assert(arma::all(optimisationProblem.getLowerBounds() <= optimisationProblem.getUpperBounds()));
    assert(!normalisedParameters.has_nan());

    return (normalisedParameters.each_col() % (optimisationProblem.getUpperBounds() - optimisationProblem.getLowerBounds())).each_col() + optimisationProblem.getLowerBounds();
  }

#if defined(SUPPORT_MPI)
  void OptimisationAlgorithm::mpiOpBestSample(void* firstInput, void* secondInput, int* size, MPI_Datatype* type) {
    // Casts both inputs to their actual type.
    double* firstSample = static_cast<double*>(firstInput);
    double* secondSample = static_cast<double*>(secondInput);

    /* Each sample contains the following information:
     * 
     * Index | Description
     * ----- | -----------------------------------
     * 0     | The number of elements (arma::uword)
     * 1     | The objective value (double)
     * 2-end | The parameter (doubles)
     */

    // Loops through multiple pairs of samples, as `MPI_User_function` requires the support of SIMD-like behaviour.
    for (int n = 0; n < *size; ++n) {
      // Corrects the type of the first value from double to `arma::uword`.
      const arma::uword numberOfDimensions = static_cast<arma::uword>(firstSample[0]);

      assert(numberOfDimensions > 0);
      assert(numberOfDimensions == static_cast<arma::uword>(secondSample[0]));

      // MPI uses the second parameter as return value.
      // In case the first parameter was actually better (had a lower objective value), copy all values (except the number of dimensions) to the second one.
      if (firstSample[1] < secondSample[1]) {
        std::copy(&firstSample[1], &firstSample[2 + numberOfDimensions], &secondSample[1]);
      }

      // Moves the pointer forward to the next pair of samples.
      firstSample += 2 + numberOfDimensions;
      secondSample += 2 + numberOfDimensions;
    }
  }
#endif
}
