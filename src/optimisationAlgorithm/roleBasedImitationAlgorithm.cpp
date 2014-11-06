#include <hop_bits/optimisationAlgorithm/roleBasedImitationAlgorithm.hpp>

// HOP
#include <hop_bits/helper/random.hpp>

namespace hop {
  RoleBasedImitationAlgorithm::RoleBasedImitationAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize)
    : OptimisationAlgorithm(optimisationProblem),
      populationSize_(populationSize),
      maximalNeighourhoodConvergence_(populationSize_),
      neighbourhoodSize_(0),
      stepSize_(arma::square((optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) / 100)) {

  }

  void RoleBasedImitationAlgorithm::optimiseImplementation() {
    arma::Mat<double> agents = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    agents.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    agents.each_col() += optimisationProblem_->getLowerBounds();

    arma::Col<double> objectiveValues(populationSize_);
    for(std::size_t n = 0; n < populationSize_; ++n) {
      ++numberOfIterations_;

      arma::Col<double> solution = agents.col(n);
      double objectiveValue = optimisationProblem_->getObjectiveValue(solution) + optimisationProblem_->getSoftConstraintsValue(solution);
      objectiveValues.at(n) = objectiveValue;
      
      if(objectiveValue < bestObjectiveValue_) {
        bestSolution_ = solution;
        bestObjectiveValue_ = objectiveValue;
      }

      if(isFinished() || isTerminated()) {
        break;
      }
    }
    
    // TODO replace all nn or nnn with k,l,m, ..
    while(!isFinished() && !isTerminated()) {
      arma::Col<arma::uword> permutation = Random::getRandomPermutation(populationSize_);
      for(std::size_t n = 0; n < populationSize_; ++n) {
        ++numberOfIterations_;

        std::size_t k = permutation.at(n);
        arma::Col<double> currentSolution = agents.col(k);
        double currentObjectiveValue = objectiveValues.at(k);

        arma::Col<arma::uword> parametersToMutate = Random::getRandomPermutation(optimisationProblem_->getNumberOfDimensions(), std::uniform_int_distribution<unsigned int>(0, optimisationProblem_->getNumberOfDimensions())(Random::Rng));

        arma::Col<arma::uword> neighbourIndicies = Random::getRandomPermutation(populationSize_ - 1, neighbourhoodSize_);
        neighbourIndicies.elem(arma::find(neighbourIndicies >= k)) += 1;

        arma::Col<double> neighbourParameters = agents.elem(neighbourIndicies);
        arma::Col<double> neighbourObjectiveValues = objectiveValues.elem(neighbourIndicies);

        double meanNeighbourFitness = arma::mean(neighbourObjectiveValues);
        arma::Col<double> meanNeighbourParameters = arma::mean(neighbourParameters.rows(parametersToMutate), 1);
        arma::Col<double> stddevNeighbourParameters = arma::stddev(neighbourParameters.rows(parametersToMutate), 1);

        arma::Col<arma::uword> betterNeighbourIndicies = arma::find(neighbourObjectiveValues < currentObjectiveValue);
        arma::Col<arma::uword> worseOrEqualNeighbourIndicies = arma::find(neighbourObjectiveValues >= currentObjectiveValue);

        if(betterNeighbourIndicies.n_elem > 0) {
          arma::Col<double> betterNeighbourFitness = neighbourObjectiveValues.elem(betterNeighbourIndicies);

          double meanBetterNeighbourFitness = arma::mean(betterNeighbourFitness);
          double stddevBetterNeighbourFitness = arma::stddev(betterNeighbourFitness);

          arma::Col<double> meanBetterParameters = arma::mean(neighbourParameters.submat(parametersToMutate, betterNeighbourIndicies), 1);
        }

        if(worseOrEqualNeighbourIndicies.n_elem > 0) {
          arma::Col<double> meanWorseParameters = arma::mean(neighbourParameters.submat(parametersToMutate, worseOrEqualNeighbourIndicies), 1);
        }

        if(betterNeighbourIndicies.n_elem > 0) {
          for(const auto& parameterToMutate : parametersToMutate) {
            if(stddevNeighbourParameters.at(parameterToMutate) < stepSize_.at(parameterToMutate)) {

            }
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
