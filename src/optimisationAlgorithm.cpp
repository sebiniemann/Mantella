#include "mantella_bits/optimisationAlgorithm.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <cassert>

namespace mant {
  OptimisationAlgorithm::OptimisationAlgorithm() {
    reset();

    setAcceptableObjectiveValue(-arma::datum::inf);
    setMaximalNumberOfIterations(std::numeric_limits<arma::uword>::max());
    setMaximalDuration(std::chrono::seconds(10));

    setBoundariesHandlingFunction(
        [this](
            const arma::Mat<double>& parameters) {
          arma::Mat<double> boundedParameters = parameters;
          
          for (arma::uword n = 0; n < parameters.n_cols; ++n) {
            static_cast<arma::Col<double>>(boundedParameters.col(n)).elem(arma::find(parameters.col(n) < 0)).fill(0);
            static_cast<arma::Col<double>>(boundedParameters.col(n)).elem(arma::find(parameters.col(n) > 1)).fill(1);
          }
          
          return boundedParameters;
        },
        "Placed on the bound.");

    isStagnatingFunction(
        [this](
            const arma::Mat<double>& parameters,
            const arma::Col<double>& objectiveValues,
            const arma::Col<double>& differences) {
          return false;
        },
        "Always false.");

    setRestartingFunction(
        [this](
            const arma::uword numberOfDimensions,
            const arma::Mat<double>& parameters,
            const arma::Col<double>& objectiveValues,
            const arma::Col<double>& differences) {
          return arma::randu<arma::Mat<double>>(arma::size(parameters));
        },
        "Randomised restart.");

#if defined(SUPPORT_MPI)
    MPI_Comm_rank(MPI_COMM_WORLD, &nodeRank_);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes_);
#else
    nodeRank_ = 0;
    numberOfNodes_ = 1;
#endif
  }

  void OptimisationAlgorithm::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::Mat<double>& initialParameters) {
    if (::mant::isVerbose) {
      std::cout << "================================================================================\n";
      std::cout << "Solving optimisation problem: " << optimisationProblem.getObjectiveFunctionName() << "\n";
      std::cout << "  Number of dimensions: " << optimisationProblem.numberOfDimensions_ << "\n";
      std::cout << "  Lower bounds: " << optimisationProblem.getLowerBounds().t();
      std::cout << "  Upper bounds: " << optimisationProblem.getUpperBounds().t();
      std::cout << "  Acceptable objective value: " << acceptableObjectiveValue_ << "\n";
      std::cout << "--------------------------------------------------------------------------------\n";
      std::cout << "  Optimisation strategy: " << nextParametersFunctionName_ << "\n";
      std::cout << "  Boundaries handling function: " << boundariesHandlingFunctionName_ << "\n";
      std::cout << "  Stagnation detection function: " << isStagnatingFunctionName_ << "\n";
      std::cout << "  Restarting function: " << restartingFunctionName_ << "\n";
      std::cout << std::endl;
    }

    reset();

    initialise(optimisationProblem.numberOfDimensions_, initialParameters);

    arma::Mat<double> parameters = boundariesHandlingFunction_(initialParameters);
    std::pair<arma::Col<double>, arma::Col<double>> objectiveValuesWithDifferences = evaluate(optimisationProblem, parameters);

    while (!isTerminated() && !isFinished()) {
      if (isStagnatingFunction_(parameters, objectiveValuesWithDifferences.first, objectiveValuesWithDifferences.second)) {
        parameters = restartingFunction_(optimisationProblem.numberOfDimensions_, parameters, objectiveValuesWithDifferences.first, objectiveValuesWithDifferences.second);
      } else {
        parameters = nextParametersFunction_(optimisationProblem.numberOfDimensions_, parameters, objectiveValuesWithDifferences.first, objectiveValuesWithDifferences.second);
      }
      assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);

      parameters = boundariesHandlingFunction_(parameters);
      assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);

      objectiveValuesWithDifferences = evaluate(optimisationProblem, parameters);

      // Communication
    }

    // Sync best parameter

    if (::mant::isVerbose) {
      if (isFinished()) {
        std::cout << "  Finished with an acceptable solution.\n";
      } else if (isTerminated()) {
        std::cout << "  Terminated (run out of time or iterations).\n";
      }

      std::cout << "    Took " << duration_.count() << " / " << maximalDuration_.count() << " milliseconds" << std::endl;
      std::cout << "    Took " << numberOfIterations_ << " / " << maximalNumberOfIterations_ << " iterations" << std::endl;
      std::cout << "    Difference to the acceptable objective value: " << bestObjectiveValue_ - acceptableObjectiveValue_ << std::endl;
      std::cout << "    Best objective value: " << bestObjectiveValue_ << "\n";
      std::cout << "    Best parameter: " << bestParameter_.t() << std::endl;
    }

    duration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialTimePoint_);
  }

  void OptimisationAlgorithm::setNextParametersFunction(
      std::function<arma::Mat<double>(const arma::uword numberOfDimensions, const arma::Mat<double>& parameters, const arma::Col<double>& objectiveValues, const arma::Col<double>& differences)> nextParametersFunction,
      const std::string& nextParametersFunctionName) {
    verify(static_cast<bool>(nextParametersFunction), "setNextParametersFunction: The next parameters function must be callable.");

    nextParametersFunction_ = nextParametersFunction;
    nextParametersFunctionName_ = nextParametersFunctionName;
  }

  void OptimisationAlgorithm::setNextParametersFunction(
      std::function<arma::Mat<double>(const arma::uword numberOfDimensions, const arma::Mat<double>& parameters, const arma::Col<double>& objectiveValues, const arma::Col<double>& differences)> nextParametersFunction) {
    setNextParametersFunction(nextParametersFunction, "Unnamed, custom next parameter function");
  }

  std::string OptimisationAlgorithm::getNextParametersFunctionName() const {
    return nextParametersFunctionName_;
  }

  void OptimisationAlgorithm::setBoundariesHandlingFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters)> boundariesHandlingFunction,
      const std::string& boundariesHandlingFunctionName) {
    verify(static_cast<bool>(boundariesHandlingFunction), "setBoundariesHandlingFunction: The boundaries handling function must be callable.");

    boundariesHandlingFunction_ = boundariesHandlingFunction;
    boundariesHandlingFunctionName_ = boundariesHandlingFunctionName;
  }

  void OptimisationAlgorithm::setBoundariesHandlingFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters)> boundariesHandlingFunction) {
    setBoundariesHandlingFunction(boundariesHandlingFunction, "Unnamed, custom boundaries handling function");
  }

  std::string OptimisationAlgorithm::getBoundariesHandlingFunctionName() const {
    return boundariesHandlingFunctionName_;
  }

  void OptimisationAlgorithm::isStagnatingFunction(
      std::function<bool(const arma::Mat<double>& parameters, const arma::Col<double>& objectiveValues, const arma::Col<double>& differences)> stagnationDetectionFunction,
      const std::string& stagnationDetectionFunctionName) {
    verify(static_cast<bool>(stagnationDetectionFunction), "isStagnatingFunction: The restart detection function must be callable.");

    isStagnatingFunction_ = stagnationDetectionFunction;
    isStagnatingFunctionName_ = stagnationDetectionFunctionName;
  }

  void OptimisationAlgorithm::isStagnatingFunction(
      std::function<bool(const arma::Mat<double>& parameters, const arma::Col<double>& objectiveValues, const arma::Col<double>& differences)> stagnationDetectionFunction) {
    isStagnatingFunction(stagnationDetectionFunction, "Unnamed, custom restart detection function");
  }

  std::string OptimisationAlgorithm::getRestartDetectionFunctionName() const {
    return isStagnatingFunctionName_;
  }

  void OptimisationAlgorithm::setRestartingFunction(
      std::function<arma::Mat<double>(const arma::uword numberOfDimensions, const arma::Mat<double>& parameters, const arma::Col<double>& objectiveValues, const arma::Col<double>& differences)> restartingFunction,
      const std::string& restartingFunctionName) {
    verify(static_cast<bool>(restartingFunction), "setRestartingFunction: The restart handling function must be callable.");

    restartingFunction_ = restartingFunction;
    restartingFunctionName_ = restartingFunctionName;
  }

  void OptimisationAlgorithm::setRestartingFunction(
      std::function<arma::Mat<double>(const arma::uword numberOfDimensions, const arma::Mat<double>& parameters, const arma::Col<double>& objectiveValues, const arma::Col<double>& differences)> restartingFunction) {
    setRestartingFunction(restartingFunction, "Unnamed, custom restart handling function");
  }

  std::string OptimisationAlgorithm::getRestartHandlingFunctionName() const {
    return restartingFunctionName_;
  }

  void OptimisationAlgorithm::setAcceptableObjectiveValue(
      const double acceptableObjectiveValue) {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
  }

  void OptimisationAlgorithm::setMaximalNumberOfIterations(
      const arma::uword maximalNumberOfIterations) {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  arma::uword OptimisationAlgorithm::getNumberOfIterations() const {
    return numberOfIterations_;
  }

  void OptimisationAlgorithm::setMaximalDuration(
      const std::chrono::microseconds maximalDuration) {
    maximalDuration_ = maximalDuration;
  }

  std::chrono::microseconds OptimisationAlgorithm::getDuration() const {
    return duration_;
  }

  bool OptimisationAlgorithm::isFinished() const {
    return bestObjectiveValue_ <= acceptableObjectiveValue_;
  }

  bool OptimisationAlgorithm::isTerminated() const {
    return numberOfIterations_ >= maximalNumberOfIterations_ || duration_ >= maximalDuration_;
  }

  double OptimisationAlgorithm::getBestObjectiveValue() const {
    return bestObjectiveValue_;
  }

  arma::Col<double> OptimisationAlgorithm::getBestParameter() const {
    return bestParameter_;
  }

  std::vector<std::pair<arma::Col<double>, double>> OptimisationAlgorithm::getSamplingHistory() const {
    return samplingHistory_;
  }

  void OptimisationAlgorithm::reset() {
    numberOfIterations_ = 0;
    bestObjectiveValue_ = arma::datum::inf;
    bestParameter_.reset();
    duration_ = std::chrono::microseconds(0);
    initialTimePoint_ = std::chrono::steady_clock::now();
  }

  void OptimisationAlgorithm::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
  }

  std::pair<arma::Col<double>, arma::Col<double>> OptimisationAlgorithm::evaluate(
      OptimisationProblem& optimisationProblem,
      const arma::Mat<double>& parameters) {
    const double previousBestObjectiveValue = bestObjectiveValue_;

    arma::Col<double> objectiveValues(parameters.n_cols);
    arma::Col<double> differences(parameters.n_cols);

    for (arma::uword n = 0; n < parameters.n_cols && !isTerminated(); ++n, ++numberOfIterations_) {
      const arma::Col<double>& parameter = parameters.col(n);

      const double objectiveValue = optimisationProblem.getNormalisedObjectiveValue(parameter);

      if (::mant::isRecordingSampling) {
        samplingHistory_.push_back({parameter, objectiveValue});
      }

      objectiveValues(n) = objectiveValue;
      differences(n) = objectiveValue - previousBestObjectiveValue;

      if (objectiveValue < bestObjectiveValue_) {
        if (::mant::isVerbose) {
          std::cout << "  Iteration #" << numberOfIterations_ << " (after " << duration_.count() << "ms) : Found better solution.\n";
          std::cout << "    Difference to the previous best objective value: " << objectiveValue - bestObjectiveValue_ << std::endl;
          std::cout << "    Best objective value: " << bestObjectiveValue_ << "\n";
          std::cout << "    Best parameter: " << bestParameter_.t() << std::endl;
        }

        bestParameter_ = parameter;
        bestObjectiveValue_ = objectiveValue;

        if (isFinished()) {
          break;
        }
      }

      duration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialTimePoint_);
    }

    return {objectiveValues, differences};
  }
}
