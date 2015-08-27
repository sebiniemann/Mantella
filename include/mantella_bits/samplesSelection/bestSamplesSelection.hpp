#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/samplesSelection.hpp>

namespace mant {
  class BestSamplesSelection : public SamplesSelection {
    public:
      using SamplesSelection::SamplesSelection;
      
      std::string toString() const override;

    protected:
      void selectImplementation() override;
  };
}
