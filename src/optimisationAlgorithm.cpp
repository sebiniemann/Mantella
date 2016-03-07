#include "mantella_bits/optimisationAlgorithm.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <cassert>
#include <iostream>
#include <limits>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  OptimisationAlgorithm::OptimisationAlgorithm() {
    reset();

    setAcceptableObjectiveValue(-arma::datum::inf);
    setMaximalNumberOfIterations(std::numeric_limits<arma::uword>::max());
    setMaximalDuration(std::chrono::seconds(1));

    setBoundariesHandlingFunction(
        [this](
            const arma::Mat<double>& parameters_,
            const arma::Mat<arma::uword>& isBelowLowerBound_,
            const arma::Mat<arma::uword>& isAboveUpperBound_) {
          assert(isBelowLowerBound_.n_rows == parameters_.n_rows);
          assert(isBelowLowerBound_.n_cols == parameters_.n_cols);
          assert(isAboveUpperBound_.n_rows == parameters_.n_rows);
          assert(isAboveUpperBound_.n_cols == parameters_.n_cols);
            
          arma::Mat<double> boundedParameters = parameters_;
          
          boundedParameters.elem(arma::find(isBelowLowerBound_)).fill(0);
          boundedParameters.elem(arma::find(isAboveUpperBound_)).fill(1);
          
          return boundedParameters;
        },
        "Map to bound");

    setIsStagnatingFunction(
        [this](
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          assert(objectiveValues_.n_elem == parameters_.n_cols);
          assert(differences_.n_elem == parameters_.n_cols);
          assert(differences_.has_inf() || arma::all(objectiveValues_ - differences_ - arma::min(objectiveValues_) + arma::min(differences_) < 1e-12 * arma::max(arma::ones<arma::Row<double>>(arma::size(objectiveValues_)), arma::abs(objectiveValues_))));
          
          return false;
        },
        "Always false");

    setRestartingFunction(
        [this](
            const arma::uword numberOfDimensions,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          assert(parameters_.n_rows == numberOfDimensions);
          assert(objectiveValues_.n_elem == parameters_.n_cols);
          assert(differences_.n_elem == parameters_.n_cols);
          assert(differences_.has_inf() || arma::all(objectiveValues_ - differences_ - arma::min(objectiveValues_) + arma::min(differences_) < 1e-12 * arma::max(arma::ones<arma::Row<double>>(arma::size(objectiveValues_)), arma::abs(objectiveValues_))));
          
          return arma::randu<arma::Mat<double>>(arma::size(parameters_));
        },
        "Random");

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
    verify(initialParameters.is_finite(), "OptimisationAlgorithm.optimise: The initial parameter must be finite.");
    verify(initialParameters.n_rows == optimisationProblem.numberOfDimensions_, "OptimisationAlgorithm.optimise: The initial parameter's number of rows must be equal to the optimisation problem's number of dimensions.");
    verify(initialParameters.n_cols > 0, "OptimisationAlgorithm.optimise: The initial parameter's number of columns must be strict greater than 0.");
    verify(arma::all(optimisationProblem.getLowerBounds() <= optimisationProblem.getUpperBounds()), "OptimisationAlgorithm.optimise: The optimisation problem's lower bounds must be less than or equal to their upper bounds.");
    verify(static_cast<bool>(nextParametersFunction_), "OptimisationAlgorithm.optimise: The next-parameter function must be callable.");

    if (::mant::isVerbose) {
      std::cout << "================================================================================\n";
      std::cout << "Solving optimisation problem: " << optimisationProblem.getObjectiveFunctionName() << "\n";
      std::cout << "  Number of dimensions: " << optimisationProblem.numberOfDimensions_ << "\n";
      std::cout << "  Lower bounds: " << optimisationProblem.getLowerBounds().t();
      std::cout << "  Upper bounds: " << optimisationProblem.getUpperBounds().t();
      std::cout << "  Acceptable objective value: " << acceptableObjectiveValue_ << "\n";
      std::cout << "--------------------------------------------------------------------------------\n";
      std::cout << "  Optimisation strategy: " << nextParametersFunctionName_ << "\n";
      std::cout << "  Boundaries-handling function: " << boundariesHandlingFunctionName_ << "\n";
      std::cout << "  Is-stagnating function: " << isStagnatingFunctionName_ << "\n";
      std::cout << "  Restarting function: " << restartingFunctionName_ << "\n";
      std::cout << std::endl;
    }

    reset();

    initialise(optimisationProblem.numberOfDimensions_, initialParameters);

    arma::Mat<double> parameters = boundariesHandlingFunction_(initialParameters, initialParameters<0.0, initialParameters> 1.0);
    assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);

    std::pair<arma::Row<double>, arma::Row<double>> objectiveValuesWithDifferences = evaluate(optimisationProblem, parameters);
    assert(objectiveValuesWithDifferences.first.n_elem == objectiveValuesWithDifferences.second.n_elem);
    assert(objectiveValuesWithDifferences.first.n_elem == parameters.n_cols);

    while (!isTerminated() && !isFinished()) {
      if (isStagnatingFunction_(parameters, objectiveValuesWithDifferences.first, objectiveValuesWithDifferences.second)) {
        parameters = restartingFunction_(optimisationProblem.numberOfDimensions_, parameters, objectiveValuesWithDifferences.first, objectiveValuesWithDifferences.second);
      } else {
        parameters = nextParametersFunction_(optimisationProblem.numberOfDimensions_, parameters, objectiveValuesWithDifferences.first, objectiveValuesWithDifferences.second);
      }
      assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);

      parameters = boundariesHandlingFunction_(parameters, parameters<0.0, parameters> 1.0);
      assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);

      objectiveValuesWithDifferences = evaluate(optimisationProblem, parameters);
      assert(objectiveValuesWithDifferences.first.n_elem == objectiveValuesWithDifferences.second.n_elem);
      assert(objectiveValuesWithDifferences.first.n_elem == parameters.n_cols);

      // Communication
    }

    // Sync best parameter

    duration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialTimePoint_);

    if (::mant::isVerbose) {
      if (isFinished()) {
        std::cout << "  Finished with an acceptable solution.\n";
      } else if (isTerminated()) {
        std::cout << "  Terminated (run out of time or iterations).\n";
      }

      std::cout << "    Took " << duration_.count() << " / " << maximalDuration_.count() << " microseconds" << std::endl;
      std::cout << "    Took " << numberOfIterations_ << " / " << maximalNumberOfIterations_ << " iterations" << std::endl;
      std::cout << "    Difference to the acceptable objective value: " << bestObjectiveValue_ - acceptableObjectiveValue_ << std::endl;
      std::cout << "    Best objective value: " << bestObjectiveValue_ << "\n";
      std::cout << "    Best parameter: " << bestParameter_.t() << std::endl;
    }

    bestParameter_ = optimisationProblem.getLowerBounds() + bestParameter_ % (optimisationProblem.getUpperBounds() - optimisationProblem.getLowerBounds());
  }

  void OptimisationAlgorithm::setNextParametersFunction(
      std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> nextParametersFunction,
      const std::string& nextParametersFunctionName) {
    verify(static_cast<bool>(nextParametersFunction), "OptimisationAlgorithm.setNextParametersFunction: The next-parameters function must be callable.");

    nextParametersFunction_ = nextParametersFunction;
    nextParametersFunctionName_ = nextParametersFunctionName;

    reset();
  }

  void OptimisationAlgorithm::setNextParametersFunction(
      std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> nextParametersFunction) {
    setNextParametersFunction(nextParametersFunction, "Unnamed, custom next-parameter function");
  }

  std::string OptimisationAlgorithm::getNextParametersFunctionName() const {
    return nextParametersFunctionName_;
  }

  void OptimisationAlgorithm::setBoundariesHandlingFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters_, const arma::Mat<arma::uword>& isOutOfLowerBound_, const arma::Mat<arma::uword>& isOutOfUpperBound_)> boundariesHandlingFunction,
      const std::string& boundariesHandlingFunctionName) {
    verify(static_cast<bool>(boundariesHandlingFunction), "OptimisationAlgorithm.setBoundariesHandlingFunction: The boundaries-handling function must be callable.");

    boundariesHandlingFunction_ = boundariesHandlingFunction;
    boundariesHandlingFunctionName_ = boundariesHandlingFunctionName;

    reset();
  }

  void OptimisationAlgorithm::setBoundariesHandlingFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters_, const arma::Mat<arma::uword>& isOutOfLowerBound_, const arma::Mat<arma::uword>& isOutOfUpperBound_)> boundariesHandlingFunction) {
    setBoundariesHandlingFunction(boundariesHandlingFunction, "Unnamed, custom boundaries-handling function");
  }

  std::string OptimisationAlgorithm::getBoundariesHandlingFunctionName() const {
    return boundariesHandlingFunctionName_;
  }

  void OptimisationAlgorithm::setIsStagnatingFunction(
      std::function<bool(const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> isStagnatingFunction,
      const std::string& isStagnatingFunctionName) {
    verify(static_cast<bool>(isStagnatingFunction), "OptimisationAlgorithm.setIsStagnatingFunction: The is-stagnating function must be callable.");

    isStagnatingFunction_ = isStagnatingFunction;
    isStagnatingFunctionName_ = isStagnatingFunctionName;

    reset();
  }

  void OptimisationAlgorithm::setIsStagnatingFunction(
      std::function<bool(const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> isStagnatingFunction) {
    setIsStagnatingFunction(isStagnatingFunction, "Unnamed, custom is-stagnation function");
  }

  std::string OptimisationAlgorithm::getIsStagnatingFunctionName() const {
    return isStagnatingFunctionName_;
  }

  void OptimisationAlgorithm::setRestartingFunction(
      std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> restartingFunction,
      const std::string& restartingFunctionName) {
    verify(static_cast<bool>(restartingFunction), "OptimisationAlgorithm.setRestartingFunction: The restarting function must be callable.");

    restartingFunction_ = restartingFunction;
    restartingFunctionName_ = restartingFunctionName;

    reset();
  }

  void OptimisationAlgorithm::setRestartingFunction(
      std::function<arma::Mat<double>(const arma::uword numberOfDimensions_, const arma::Mat<double>& parameters_, const arma::Row<double>& objectiveValues_, const arma::Row<double>& differences_)> restartingFunction) {
    setRestartingFunction(restartingFunction, "Unnamed, custom restarting function");
  }

  std::string OptimisationAlgorithm::getRestartingFunctionName() const {
    return restartingFunctionName_;
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
    verify(maximalNumberOfIterations > 0, "OptimisationAlgorithm.setMaximalNumberOfIterations: The maximal number of iterations must be strict greater than 0.");

    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  arma::uword OptimisationAlgorithm::getMaximalNumberOfIterations() const {
    return maximalNumberOfIterations_;
  }

  void OptimisationAlgorithm::setMaximalDuration(
      const std::chrono::microseconds maximalDuration) {
    verify(maximalDuration.count() > 0, "OptimisationAlgorithm.setMaximalDuration: The maximal duration must be strict greater than 0 millisecond.");

    maximalDuration_ = maximalDuration;
  }

  std::chrono::microseconds OptimisationAlgorithm::getMaximalDuration() const {
    return maximalDuration_;
  }

  arma::uword OptimisationAlgorithm::getNumberOfIterations() const {
    return numberOfIterations_;
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

  std::vector<std::pair<arma::Col<double>, double>> OptimisationAlgorithm::getRecordedSampling() const {
    return recordedSampling_;
  }

  void OptimisationAlgorithm::reset() {
    numberOfIterations_ = 0;
    bestObjectiveValue_ = arma::datum::inf;
    bestParameter_.reset();
    duration_ = std::chrono::microseconds(0);
    initialTimePoint_ = std::chrono::steady_clock::now();

    recordedSampling_.clear();
  }

  void OptimisationAlgorithm::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
  }

  std::pair<arma::Row<double>, arma::Row<double>> OptimisationAlgorithm::evaluate(
      OptimisationProblem& optimisationProblem,
      const arma::Mat<double>& parameters) {
    const double previousBestObjectiveValue = bestObjectiveValue_;

    arma::Row<double> objectiveValues(parameters.n_cols);
    arma::Row<double> differences(parameters.n_cols);

    for (arma::uword n = 0; n < parameters.n_cols && !isTerminated(); ++n) {
      const arma::Col<double>& parameter = parameters.col(n);

      const double objectiveValue = optimisationProblem.getNormalisedObjectiveValue(parameter);

      if (::mant::isRecordingSampling) {
        recordedSampling_.push_back({parameter, objectiveValue});
      }

      objectiveValues(n) = objectiveValue;
      differences(n) = objectiveValue - previousBestObjectiveValue;

      duration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialTimePoint_);

      if (objectiveValue < bestObjectiveValue_) {
        if (::mant::isVerbose) {
          std::cout << "  Iteration #" << numberOfIterations_ << " (after " << duration_.count() << "ms) : Found better solution.\n";
          std::cout << "    Difference to the previous best objective value: " << objectiveValue - bestObjectiveValue_ << std::endl;
          std::cout << "    Best objective value: " << objectiveValue << "\n";
          std::cout << "    Best parameter: " << parameter.t() << std::endl;
        }

        bestParameter_ = parameter;
        bestObjectiveValue_ = objectiveValue;

        if (isFinished()) {
          break;
        }
      }
    }
    ++numberOfIterations_;

    return {objectiveValues, differences};
  }
}
