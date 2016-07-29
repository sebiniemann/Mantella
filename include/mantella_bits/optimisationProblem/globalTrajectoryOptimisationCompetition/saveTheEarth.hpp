#pragma once

// C++ standard library
#include <tuple>
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  namespace gtoc {
    class SaveTheEarth : public OptimisationProblem {
     public:
      const std::vector<std::tuple<arma::uword, bool, arma::uword>> solarBodiesSequence_;

      SaveTheEarth(
          std::vector<std::tuple<arma::uword, bool, arma::uword>> solarBodiesSequence);

     protected:
      arma::mat::fixed<7, 7> keplerianElements_;
      arma::vec::fixed<6> standardGravitationalParameters_;
      arma::vec::fixed<6> minimalPeriapsides_;
      arma::vec::fixed<6> masses_;
    };
  }
}