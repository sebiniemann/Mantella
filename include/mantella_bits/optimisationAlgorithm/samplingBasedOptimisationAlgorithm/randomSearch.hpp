#pragma once

// C++ standard library
#include <string>

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
