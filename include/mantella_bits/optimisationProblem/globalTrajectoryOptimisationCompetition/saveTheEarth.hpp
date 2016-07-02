#pragma once

// Mantella
#include "mantella_bits/optimisationProblem/globalTrajectoryOptimisationCompetition.hpp"

namespace mant {
  namespace gtoc {
    class SaveTheEarth : public GlobalTrajectoryOptimisationCompetition  {
      public:
        const std::vector<std::tupel<arma::uword, bool, arma::uword>> solarBodiesSequence_;
      
        SaveTheEarth(
            std::vector<std::tupel<arma::uword, bool, arma::uword>> solarBodiesSequence);
            
      protected:
        arma::mat::fixed<8, 6> keplerianElements_;
        arma::vec::fixed<7> standardGravitationalParameters_:
        arma::vec::fixed<7> minimalPeriapsides_;
        arma::vec::fixed<7> masses_;
    }
  }
}