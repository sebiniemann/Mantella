#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/printable.hpp>
#include <mantella_bits/helper/unorderedContainer.hpp>

namespace mant {
  class SamplesSelection : public Printable {
    public:
      SamplesSelection(
          std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples,
          arma::uword numberOfSelectedSamples);
      
      void select();
      
       std::unordered_map<arma::Col<double>, double, Hash, IsEqual> getSelectedSamples() const;
      
      protected:
        std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples_;
        
        arma::uword numberOfSelectedSamples_;
        std::unordered_map<arma::Col<double>, double, Hash, IsEqual> selectedSamples_;
        
        virtual void selectImplementation() = 0;
      virtual ~SamplesSelection() = default;
  };
}