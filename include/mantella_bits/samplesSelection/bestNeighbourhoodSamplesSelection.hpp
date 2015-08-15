#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/samplesSelection.hpp>

namespace mant {
  class BestNeighbourhoodSamplesSelection : public SamplesSelection {
    public:
      std::string toString() const override;

    protected:
      void selectImplementation() override;
  };
}