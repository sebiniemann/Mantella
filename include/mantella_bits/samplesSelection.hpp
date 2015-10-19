#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/helper/printable.hpp"
namespace mant {
  class Hash;
  class IsEqual;
}

namespace mant {
  class SamplesSelection : public Printable {
   public:
    explicit SamplesSelection(
        std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples,
        arma::uword numberOfSelectedSamples);

    void select();

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> getSelectedSamples() const;

    virtual ~SamplesSelection() = default;

   protected:
    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples_;

    arma::uword numberOfSelectedSamples_;
    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> selectedSamples_;

    virtual void selectImplementation() = 0;
  };
}
