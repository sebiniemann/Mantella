#include "mantella_bits/optimisationProblem/globalTrajectoryOptimisationCompetition/saveTheEarth.hpp"

// C++ standard library
#include <cassert>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/config.hpp"
#include "mantella_bits/optimisationAlgorithm/hookeJeevesAlgorithm.hpp"
#include "mantella_bits/orbitalMechanics.hpp"

namespace mant {
  namespace gtoc {
    SaveTheEarth::SaveTheEarth(
        std::vector<std::tuple<arma::uword, bool, arma::uword>> solarBodiesSequence) 
      : GlobalTrajectoryOptimisationCompetition(solarBodiesSequence.size()),
        solarBodiesSequence_(solarBodiesSequence),
        keplerianElements_({5.790989450065017e10, 0.205630251208908, 0.498330875729913, 1.179196004039061, 0.191775863495282, 3.015051280320217, 4.4534016e9, 1.082096967113709e11, 0.006755697267164, 0.059246765838336, 0.963167183054409, 1.338290119889351, 0.860696061867345, 4.4534016e9, 1.496556263647797e11, 0.017042397167815, 4.658482428234955e-06, 5.197011175839376, 2.861907905845351, 6.251579845252470, 4.4534016e9, 2.279422563716072e11, 0.093314606537239, 0.430696475559172, 5.812267756657162, 0.058881884779006, 0.333262048832696, 4.4534016e9, 7.786842440087036e11, 0.048923059357254, 0.022770571412988, 4.801742885804805, 1.753863276392004, 0.326811615782352, 4.4534016e9, 1.433385218061753e12, 0.055599288974876, 0.043360415404928, 5.862572285981615, 1.984317417175440, 5.591683655893863, 4.4534016e9, 2.5897261, 0.2734625, 6.40734, 128.34711, 264.78691, 320.479555, 4.4534016e9}),
        standardGravitationalParameters_({2.20329e13, 3.248599e14, 3.9860044189e14, 4.2828372e13, 1.266865349e17, 3.79311879e16}),
        minimalPeriapsides_({2740000.0, 6351000.0, 6678000.0, 3689000.0, 600000000.0, 70000000.0}), 
        masses_({3.302e23, 4.867e24, 5.972e24, 6.39e23, 1.898e27, 5.683e26}) {
      if (solarBodiesSequence.empty()) {
        throw std::domain_error("SaveTheEarth: The solar bodies sequence size must not be empty.");
      } else if(std::get<0>(solarBodiesSequence.back()) != 6) {
        throw std::domain_error("SaveTheEarth: The last solar body in the sequence must be the asteroid 2001 TW229.");
      } else if (std::find_if(solarBodiesSequence.cbegin(), solarBodiesSequence.cend(), [](const auto& element) {return std::get<0>(element) == 6;}) != --solarBodiesSequence.end()) {
        throw std::domain_error("SaveTheEarth: The asteroid 2001 TW229 must only be at the end of the solar bodies sequence.");
      }
      
      setLowerBounds(arma::join_cols(arma::vec({4769020800.0}), arma::zeros<arma::vec>(numberOfDimensions_ - 1) + 0.0));
      setUpperBounds(arma::join_cols(arma::vec({5400172800.0}), arma::zeros<arma::vec>(numberOfDimensions_ - 1) + 946080000.0));

      setObjectiveFunctions({{[this](
          const arma::vec& parameter) {
        assert(launchAndTransferTimes.n_elem == numberOfDimensions_);
        
        const double timeOfTrip = arma::accu(parameter.tail(numberOfDimensions_ - 1));
        if (timeOfTrip > 946080000.0) {
          return 946080000.0 - timeOfTrip;
        }
        
        arma::vec launchAndTransferTimes = parameter;
        launchAndTransferTimes.tail(numberOfDimensions_ - 1) += arma::cumsum(launchAndTransferTimes.tail(numberOfDimensions_ - 1)) + launchAndTransferTimes(0);
        
        double deltaV = 0.0;
        
        std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> previousSolarBodyPositionAndVelocity = positionAndVelocityOnOrbit(launchAndTransferTimes(0), keplerianElements_.col(2));
        std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> currentSolarBodyPositionAndVelocity = positionAndVelocityOnOrbit(launchAndTransferTimes(1), keplerianElements_.col(std::get<0>(solarBodiesSequence_.at(0))));
        std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> previousToCurrentSolarBodySatelliteVelocities = lambert(previousSolarBodyPositionAndVelocity.first, currentSolarBodyPositionAndVelocity.first, std::get<1>(solarBodiesSequence_.at(0)), std::get<2>(solarBodiesSequence_.at(0)), launchAndTransferTimes(1) - launchAndTransferTimes(0));
        
        deltaV += arma::norm(previousSolarBodyPositionAndVelocity.second - previousToCurrentSolarBodySatelliteVelocities.first);
        
        OptimisationProblem optimisationProblem(1);
        HookeJeevesAlgorithm optimisationAlgorithm;
        optimisationAlgorithm.setMaximalNumberOfIterations(100);
        
        std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> nextSolarBodyPositionAndVelocity;
        std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> currentToNextSolarBodySatelliteVelocities;
        for (arma::uword n = 1; n < numberOfDimensions_ - 1; ++n) {
          nextSolarBodyPositionAndVelocity = positionAndVelocityOnOrbit(launchAndTransferTimes(n + 1), keplerianElements_.col(std::get<0>(solarBodiesSequence_.at(n))));
          currentToNextSolarBodySatelliteVelocities = lambert(currentSolarBodyPositionAndVelocity.first, nextSolarBodyPositionAndVelocity.first, std::get<1>(solarBodiesSequence_.at(n)), std::get<2>(solarBodiesSequence_.at(n)), launchAndTransferTimes(n + 1) - launchAndTransferTimes(n));
          
          auto gravityAssistFunction = std::bind(gravityAssist, previousToCurrentSolarBodySatelliteVelocities.second, currentSolarBodyPositionAndVelocity.first, currentSolarBodyPositionAndVelocity.second, standardGravitationalParameters_(std::get<0>(solarBodiesSequence_.at(n - 1))), std::placeholders::_1);
        
          optimisationProblem.setLowerBounds({minimalPeriapsides_(std::get<0>(solarBodiesSequence_.at(n - 1)))});
          optimisationProblem.setUpperBounds({sphereOfInfluenceRadius(arma::norm(currentSolarBodyPositionAndVelocity.first), masses_(std::get<0>(solarBodiesSequence_.at(n))))});
          optimisationProblem.setObjectiveFunctions({{
            [&currentToNextSolarBodySatelliteVelocities, &gravityAssistFunction] (const arma::vec& parameter) {
            return arma::norm(gravityAssistFunction(parameter(0)) - currentToNextSolarBodySatelliteVelocities.first);
          }, "Difference betwenn actual and desired velocity"}});
          optimisationAlgorithm.optimise(optimisationProblem);
          
          deltaV += optimisationAlgorithm.getBestFoundObjectiveValue();
          
          previousSolarBodyPositionAndVelocity = currentSolarBodyPositionAndVelocity;
          currentSolarBodyPositionAndVelocity = nextSolarBodyPositionAndVelocity;
        }
        
        return 1500.0 * std::exp(-deltaV / 24.516625) * std::abs(arma::dot(currentToNextSolarBodySatelliteVelocities.second, nextSolarBodyPositionAndVelocity.second));
        }, "GTOC Save the earth (first)"}});
    }
  }
}