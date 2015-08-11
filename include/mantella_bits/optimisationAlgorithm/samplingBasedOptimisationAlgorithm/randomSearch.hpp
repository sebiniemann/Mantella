#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationAlgorithm/samplingBasedOptimisationAlgorithm.hpp>

namespace mant {
  class RandomSearch : public SamplingBasedOptimisationAlgorithm {
    public:
      using SamplingBasedOptimisationAlgorithm::SamplingBasedOptimisationAlgorithm;

      std::string toString() const override;

    protected:
      void optimiseImplementation() override;
  };
}
