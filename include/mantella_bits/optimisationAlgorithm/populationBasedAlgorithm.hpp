#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm.hpp>

namespace mant {
  template <typename ParameterType>
  class PopulationBasedAlgorithm : public OptimisationAlgorithm<ParameterType> {
    public:
      explicit PopulationBasedAlgorithm(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
          const unsigned int& populationSize) noexcept;

      void setInitialPopulation(
          const arma::Mat<ParameterType>& initialPopulation);

    protected:
      unsigned int populationSize_;
      arma::Mat<ParameterType> initialPopulation_;
  };

  template <typename ParameterType>
  PopulationBasedAlgorithm<ParameterType>::PopulationBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
      const unsigned int& populationSize) noexcept
    : OptimisationAlgorithm<ParameterType>(optimisationProblem),
      populationSize_(populationSize) {
    // TODO fix for discrete problems
    arma::Mat<ParameterType> population = arma::randu<arma::Mat<ParameterType>>(OptimisationAlgorithm<ParameterType>::optimisationProblem_->getNumberOfDimensions(), populationSize_);
    population.each_col() %= OptimisationAlgorithm<ParameterType>::optimisationProblem_->getUpperBounds() - OptimisationAlgorithm<ParameterType>::optimisationProblem_->getLowerBounds();
    population.each_col() += OptimisationAlgorithm<ParameterType>::optimisationProblem_->getLowerBounds();

    setInitialPopulation(population);
  }

  template <typename ParameterType>
  void PopulationBasedAlgorithm<ParameterType>::setInitialPopulation(
      const arma::Mat<ParameterType>& initialPopulation) {
    if(initialPopulation.n_rows != OptimisationAlgorithm<ParameterType>::optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the each parameter (" + std::to_string(initialPopulation.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(OptimisationAlgorithm<ParameterType>::optimisationProblem_->getNumberOfDimensions()) + ").");
    }

    initialPopulation_ = initialPopulation;
    populationSize_ = initialPopulation.n_cols;
  }
}
