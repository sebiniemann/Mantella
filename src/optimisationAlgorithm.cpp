#include "mantella_bits/optimisationAlgorithm.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <cassert>

namespace mant{
  OptimisationAlgorithm::OptimisationAlgorithm() {
    reset();
        
    setAcceptableObjectiveValue(-arma::datum::inf);
    setMaximalNumberOfIterations(std::numeric_limits<arma::uword>::max());
    setMaximalDuration(std::chrono::seconds(10));
    
    setBoundaryHandlingFunction([this] (
        const arma::Mat<double>& parameters) {
      arma::Mat<double> boundedParameters = parameters;
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        static_cast<arma::Col<double>>(boundedParameters.col(n)).elem(arma::find(parameters.col(n) < 0)).fill(0);
        static_cast<arma::Col<double>>(boundedParameters.col(n)).elem(arma::find(parameters.col(n) > 1)).fill(1);
      }
      
      return boundedParameters;
    });
    
    setIsDegeneratedFunction([this] (
        const arma::Mat<double>& parameters,
        const arma::Col<double>& differences) {
      return false;
    });
    
    setDegenerationHandlingFunction([this] (
        const arma::Mat<double>& parameters,
        const arma::Col<double>& differences) {
      return arma::randu<arma::Mat<double>>(arma::size(parameters));
    });
    
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
      std::cout << "Solving optimisation problem: " << optimisationProblem.getName() << "\n";
      std::cout << "  Number of dimensions: " << optimisationProblem.numberOfDimensions_ << "\n";
      std::cout << "  Lower bounds: " << optimisationProblem.getLowerBounds().t();
      std::cout << "  Upper bounds: " << optimisationProblem.getUpperBounds().t();
      std::cout << "  Acceptable objective value: " << acceptableObjectiveValue_ << "\n" << std::endl;
    }
    
    reset();
    
    arma::Mat<double> parameters = boundaryHandlingFunction_(initialParameters);
    arma::Col<double> differences = evaluate(optimisationProblem, parameters);
    
    while (!isTerminated() && !isFinished()) {
      if (isDegeneratedFunction_(parameters, differences)) {
        parameters = degenerationHandlingFunction_(parameters, differences);
      } else {
        parameters = nextParametersFunction_(parameters, differences);
      }
      assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);
      
      parameters = boundaryHandlingFunction_(parameters);
      assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);
      
      differences = evaluate(optimisationProblem, parameters);
      
      // Communication
      
    }
    
    // Sync best parameter
    
        
    if (::mant::isVerbose) {
      if (isFinished()) {
        std::cout << "  Finished with an acceptable solution.\n";
      } else if (isTerminated()) {
        std::cout << "  Terminated (run out of time or iterations).\n";
      }
      
      std::cout << "    Took " << duration_.count() << " / " << maximalDuration_.count() << " milliseconds" <<std::endl;
      std::cout << "    Took " << numberOfIterations_ << " / " << maximalNumberOfIterations_ << " iterations" <<std::endl;
      std::cout << "    Difference to the acceptable objective value: " << bestObjectiveValue_ - acceptableObjectiveValue_ << std::endl;
      std::cout << "    Best objective value: " << bestObjectiveValue_ << "\n";
      std::cout << "    Best parameter: " << bestParameter_.t() << std::endl;
    }
        
    duration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialTimePoint_);
  }

  void OptimisationAlgorithm::setNextParametersFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> nextParametersFunction) {
    // Using the *operator bool* to checks whether *objectiveFunction_* is empty (not callable) or not.
    verify(static_cast<bool>(nextParametersFunction), "setNextParametersFunction: The next parameters function must be callable.");
    
    nextParametersFunction_ = nextParametersFunction;
  }
  
  void OptimisationAlgorithm::setBoundaryHandlingFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters)> boundaryHandlingFunction) {
    // Using the *operator bool* to checks whether *objectiveFunction_* is empty (not callable) or not.
    verify(static_cast<bool>(boundaryHandlingFunction), "setBoundaryHandlingFunction: The boundary handling function must be callable.");
    
    boundaryHandlingFunction_ = boundaryHandlingFunction;
  }
  
  void OptimisationAlgorithm::setIsDegeneratedFunction(
      std::function<bool(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> isDegeneratedFunction) {
    // Using the *operator bool* to checks whether *objectiveFunction_* is empty (not callable) or not.
    verify(static_cast<bool>(isDegeneratedFunction), "setIsDegeneratedFunction: The function determining whether the state is degenerated must be callable.");
    
    isDegeneratedFunction_ = isDegeneratedFunction;
  }
  
  void OptimisationAlgorithm::setDegenerationHandlingFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationHandlingFunction) {
    // Using the *operator bool* to checks whether *objectiveFunction_* is empty (not callable) or not.
    verify(static_cast<bool>(degenerationHandlingFunction), "setDegenerationHandlingFunction: The degenerative handling function must be callable.");
    
    degenerationHandlingFunction_ = degenerationHandlingFunction;
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
  
  arma::Col<double> OptimisationAlgorithm::evaluate(
      OptimisationProblem& optimisationProblem,
      const arma::Mat<double>& parameters) {
    arma::Col<double> differences(parameters.n_cols);
    for (arma::uword n = 0; n < parameters.n_cols && !isTerminated(); ++n, ++numberOfIterations_) {
      const arma::Col<double>& parameter = parameters.col(n);
    
      const double objectiveValue = optimisationProblem.getNormalisedObjectiveValue(parameter);
      const double difference = objectiveValue - bestObjectiveValue_;
      
      if (::mant::isRecordingSampling) {
        samplingHistory_.push_back({parameter, objectiveValue});
      }
      
      differences(n) = difference;
      
      if (difference < 0) {
        bestParameter_ = parameter;
        bestObjectiveValue_ = objectiveValue;
        
        if (::mant::isVerbose) {
          std::cout << "  Iteration #" << numberOfIterations_ << " (after " << duration_.count() << "ms) : Found better solution.\n";
          std::cout << "    Difference to the previous best objective value: " << difference << std::endl;
          std::cout << "    Best objective value: " << bestObjectiveValue_ << "\n";
          std::cout << "    Best parameter: " << bestParameter_.t() << std::endl;
        }
        
        if (isFinished()) {
          break;
        }
      }
      
      duration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialTimePoint_);
    }

    return differences;
  }
  
  void OptimisationAlgorithm::reset() {
    numberOfIterations_  = 0;
    bestObjectiveValue_ = arma::datum::inf;
    bestParameter_.reset();
    duration_ = std::chrono::microseconds(0);
    initialTimePoint_ = std::chrono::steady_clock::now();
  }
}
