#pragma once

// Mantella
#include "mantella_bits/optimisationProblem/globalTrajectoryOptimisationCompetition.hpp"

namespace mant {
  namespace gtoc {
    class SaveTheEarth : public GlobalTrajectoryOptimisationCompetition {
      public:
        const std::vector<std::tuple<arma::uword, bool, arma::uword>> solarBodiesSequence_;
      
        SaveTheEarth(
            std::vector<std::tuple<arma::uword, bool, arma::uword>> solarBodiesSequence);
            
      protected:
        arma::mat::fixed<7, 6> keplerianElements_;
        arma::vec::fixed<6> standardGravitationalParameters_;
        arma::vec::fixed<6> minimalPeriapsides_;
        arma::vec::fixed<6> masses_;
    };
  }
}