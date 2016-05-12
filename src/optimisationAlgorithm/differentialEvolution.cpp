#include "mantella_bits/optimisationAlgorithm/differentialEvolution.hpp"

// C++ standard library
#include <cstdlib>
#include <random>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  DifferentialEvolution::DifferentialEvolution()
      : OptimisationAlgorithm(),
        populationSize_(0),
        amplificationFactor_(arma::datum::nan),
        crossoverConstant_(arma::datum::nan),
        mutationFunction_(
            [this](
                arma::uword dim) {
                
              return arma::datum::nan;
            }) {
    setNextParametersFunction(
        [this](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
        
          population_.col(populationIndex_) = parameters_.col(populationIndex_);  
          
          if(objectiveValues_(populationIndex_) < localBestObjectiveValues_(populationIndex_)) {
            localBestSolutions_.col(populationIndex_) = parameters_.col(populationIndex_);
            localBestObjectiveValues_(populationIndex_) = objectiveValues_(populationIndex_);
          }
              
          std::uniform_real_distribution<double> randomValue(0.0, 1.0);
          std::default_random_engine randomEngine;
          
          arma::Col<double> mutatedParameter = parameters_.col(populationIndex_);
              
          for(arma::uword dim = 0; dim < numberOfDimensions_; dim++){            
            if(randomValue(randomEngine) < crossoverConstant_ || populationIndex_ == static_cast<unsigned long long>(std::rand()) % populationSize_) {
              // Dimensional mutation
              mutatedParameter(dim, populationIndex_) += mutationFunction_(dim);                 
            }
          }
          
          return mutatedParameter.col(populationIndex_);
        },
        "Differential evolution");

    setPopulationSize(20);
    setAmplificationFactor(0.8);
    setCrossoverConstant(0.8);
    setMutationFunction(
        [this](
            arma::uword dim) {  
          arma::uword firstRandomMember = static_cast<unsigned long long>(std::rand()) % populationSize_;
          arma::uword secondRandomMember = static_cast<unsigned long long>(std::rand()) % populationSize_;
          
          if(firstRandomMember == secondRandomMember){
            secondRandomMember = (secondRandomMember + 1) % populationSize_; 
          }
          return amplificationFactor_ * (localBestSolutions_(dim, firstRandomMember) - localBestSolutions_(dim, secondRandomMember));
        });
  }

  void DifferentialEvolution::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    populationIndex_ = 0;
    population_ = arma::randu(numberOfDimensions, populationSize_); //TODO: multiplicate bounds

    localBestSolutions_ = population_;
    localBestObjectiveValues_.set_size(populationSize_);
    localBestObjectiveValues_.fill(arma::datum::inf);
  }

  void DifferentialEvolution::optimise(
      OptimisationProblem& optimisationProblem) {
    OptimisationAlgorithm::optimise(optimisationProblem, arma::randu<arma::Col<double>>(optimisationProblem.numberOfDimensions_));
  }

  void DifferentialEvolution::setPopulationSize(
      const arma::uword populationSize) {
    populationSize_ = populationSize;
  }

  arma::uword DifferentialEvolution::getPopulationSize()
      const {
    return populationSize_;
  }

  void DifferentialEvolution::setAmplificationFactor(
      const double amplificationFactor) {
    amplificationFactor_ = amplificationFactor;
  }

  double DifferentialEvolution::getAmplificationFactor()
      const {
    return amplificationFactor_;
  }

  void DifferentialEvolution::setCrossoverConstant(
      const double crossoverConstant) {
    crossoverConstant_ = crossoverConstant;
  }

  double DifferentialEvolution::getCrossoverConstant()
      const {
    return crossoverConstant_;
  }

  void DifferentialEvolution::setMutationFunction(
      const std::function<double(arma::uword)> mutationFunction) {
    mutationFunction_ = mutationFunction;
  }

  std::function<double(arma::uword)> DifferentialEvolution::getMutationFunction()
      const {
    return mutationFunction_;
  }
}
