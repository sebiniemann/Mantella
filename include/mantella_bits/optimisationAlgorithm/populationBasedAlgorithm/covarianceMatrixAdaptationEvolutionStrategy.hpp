#pragma once

#include <mantella_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  class CovarianceMatrixAdaptationEvolutionStrategy : public PopulationBasedAlgorithm<double, EuclideanDistance> {
    public:
      //TODO: which input parameters should be added to the constructor?
      explicit CovarianceMatrixAdaptationEvolutionStrategy(
        const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,const unsigned int& populationSize);

      CovarianceMatrixAdaptationEvolutionStrategy(const CovarianceMatrixAdaptationEvolutionStrategy&) = delete;
      CovarianceMatrixAdaptationEvolutionStrategy& operator=(const CovarianceMatrixAdaptationEvolutionStrategy&) = delete;

      std::string to_string() const noexcept override;

      void setStepsize(double sigma);

    protected:
      void optimiseImplementation() override;
      double stepSize_;
  };
}
