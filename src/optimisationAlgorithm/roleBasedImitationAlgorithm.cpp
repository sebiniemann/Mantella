#include <hop_bits/optimisationAlgorithm/roleBasedImitationAlgorithm.hpp>

#include <hop_bits/helper/random.hpp>

namespace hop {
  RoleBasedImitationAlgorithm::RoleBasedImitationAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize)
    : OptimisationAlgorithm(optimisationProblem),
      populationSize_(populationSize),
      objectiveValues_(populationSize_) {

  }

  void RoleBasedImitationAlgorithm::optimiseImplementation() {
    agents_ = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    agents_.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    agents_.each_col() += optimisationProblem_->getLowerBounds();

    for(std::size_t n = 0; n < populationSize_; ++n) {
      ++numberOfIterations_;

      arma::Col<double> solution = agents_.col(n);
      double objectiveValue = optimisationProblem_->getObjectiveValue(solution) + optimisationProblem_->getSoftConstraintsValue(solution);
      objectiveValues_.at(n) = objectiveValue;
      
      if(objectiveValue < bestObjectiveValue_) {
        bestSolution_ = solution;
        bestObjectiveValue_ = objectiveValue;
      }

      if(isFinished() || isTerminated()) {
        break;
      }
    }
    
    while(!isFinished() && !isTerminated()) {
      arma::Col<arma::uword> permutation = Random::getRandomPermutation(populationSize_);
      for(std::size_t n = 0; n < populationSize_; ++n) {
        ++numberOfIterations_;

        std::size_t nn = permutation.at(n);
        arma::Col<double> currentSolution = agents_.col(nn);
        double currentObjectiveValue = objectiveValues_.at(nn);
        
        arma::Col<arma::uword> neighbours = Random::getRandomPermutation(populationSize_ - 1, neighbourhoodSize_);
        neighbours.elem(arma::find(neighbours >= nn)) += 1;

        arma::Col<arma::uword> parametersToMutate = Random::getRandomPermutation(optimisationProblem_->getNumberOfDimensions(), std::uniform_int_distribution<unsigned int>(0, optimisationProblem_->getNumberOfDimensions())(Random::Rng));

        arma::Col<double> neighourhoodMeanObjectiveValues = arma::mean(static_cast<arma::Mat<double>>(agents_.cols(neighbours)).rows(parametersToMutate));

        arma::Col<arma::uword> betterNeighbours = arma::find(objectiveValues_.elem(neighbours) < currentObjectiveValue);

        if(betterNeighbours.n_elem > 0) {
          arma::Col<double> neighbourhoodStddevObjectiveValues = arma::stddev(static_cast<arma::Mat<double>>(agents_.cols(neighbours)).rows(parametersToMutate));
          arma::Col<arma::uword> convergedNeighbours = arma::find(neighbourhoodStddevObjectiveValues < maximalNeighourhoodConvergence_.elem(parametersToMutate));

          if(convergedNeighbours.n_elem > 0) {

          }
        } else {

        }
        //                if convergedSize > 0

        //                    convergedParameter = parametersToMutate(converged);

        //                    % notNeighbours = setdiff(agents, [neighbours(:); agent]);
        //                    notNeighbours = agents(~ismembc(agents, sort([neighbours(:); agent])));

        //                    factor = ones(1, convergedSize);
        //                    factor(randperm(convergedSize, randi(convergedSize))) = -1;

        //                    p = rand(1, convergedSize);

        //                    other_k = parameters(sub2ind(size(parameters), notNeighbours(randi(size(notNeighbours, 2), 1, convergedSize)), convergedParameter));
        //                    other_j = parameters(sub2ind(size(parameters), notNeighbours(randi(size(notNeighbours, 2), 1, convergedSize)), convergedParameter));
        //                    x = other_k .* (1 - p) + other_j .* p;

        //                    parametersCandidate = x + factor .* abs(x - factor .* max(factor .* other_k, factor .* other_j)) .* rand(1, convergedSize) * 2;
        //                    withinBoundary = parametersCandidate >= lowerBounds(convergedParameter) & parametersCandidate <= upperBounds(convergedParameter);
        //                    parameters(agent, convergedParameter(withinBoundary)) = parametersCandidate(withinBoundary);
        //                else
        //                    static = true;
        //                end
      }
    }
  }

  void RoleBasedImitationAlgorithm::setNeighbourhoodSize(const unsigned int& neighbourhoodSize) {
    neighbourhoodSize_ = neighbourhoodSize;
  }

  void RoleBasedImitationAlgorithm::setMaximalNeighourhoodConvergence(const arma::Col<double>& maximalNeighourhoodConvergence) {
    maximalNeighourhoodConvergence_ = maximalNeighourhoodConvergence;
  }

  std::string RoleBasedImitationAlgorithm::to_string() const {
    return "RoleBasedImitationAlgorithm";
  }
}
