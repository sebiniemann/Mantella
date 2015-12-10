#include "mantella_bits/optimisationAlgorithm.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <cassert>

namespace mant{
  OptimisationAlgorithm::OptimisationAlgorithm() {
    reset();
        
    setAcceptableObjectiveValue(std::numeric_limits<double>::max());
    setMaximalNumberOfIterations(std::numeric_limits<arma::uword>::max());
    setMaximalDuration(std::chrono::seconds(30));
    
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
    reset();
    
    arma::Mat<double> parameters = initialParameters;
    arma::Col<double> differences = evaluate(optimisationProblem, boundaryHandlingFunction_(parameters));
    
    while (!isTerminated() && !isFinished()) {
      if (isDegeneratedFunction_(parameters, differences)) {
        parameters = degenerationHandlingFunction_(parameters, differences);
      } else {
        parameters = nextParametersFunction_(parameters, differences);
      }
      assert(parameters.n_rows == optimisationProblem.numberOfDimensions_);
      
      differences = evaluate(optimisationProblem, boundaryHandlingFunction_(parameters));
      
      // Communication
      
    }
    
    // Sync best parameter
    
    duration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialTimePoint_);
  }

  void OptimisationAlgorithm::setNextParametersFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> nextParametersFunction) {
    nextParametersFunction_ = nextParametersFunction;
  }
  
  void OptimisationAlgorithm::setBoundaryHandlingFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters)> boundaryHandlingFunction) {
    boundaryHandlingFunction_ = boundaryHandlingFunction;
  }
  
  void OptimisationAlgorithm::setIsDegeneratedFunction(
      std::function<bool(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> isDegeneratedFunction) {
    isDegeneratedFunction_ = isDegeneratedFunction;
  }
  
  void OptimisationAlgorithm::setDegenerationHandlingFunction(
      std::function<arma::Mat<double>(const arma::Mat<double>& parameters, const arma::Col<double>& differences)> degenerationHandlingFunction) {
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
    
      const double objectiveValue = optimisationProblem.getObjectiveValue(parameter);
      const double difference = objectiveValue - bestObjectiveValue_;
      
      if (::mant::recordSamplingHistory) {
        samplingHistory_.push_back({parameter, objectiveValue});
      }
      
      differences(n) = difference;
      
      if (difference < 0) {
        bestParameter_ = parameter;
        bestObjectiveValue_ = objectiveValue;
        
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
    bestObjectiveValue_ = std::numeric_limits<double>::max();
    bestParameter_.reset();
    duration_ = std::chrono::microseconds(0);
    initialTimePoint_ = std::chrono::steady_clock::now();
  }
}
