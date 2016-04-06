#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/numericalAnalysis.hpp"
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/optimisationAlgorithm/hookeJeevesAlgorithm.hpp"

namespace mant {
  namespace itd {

    std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> lambert(
        const arma::Col<double>::fixed<3>& departurePosition,
        const arma::Col<double>::fixed<3>& arrivalPosition,
        const double transferTime) {
      verify(transferTime > 0.0, "lambert: The transfer time must be greater than zero.");

      double flightDistance = arma::norm(departurePosition - arrivalPosition);
      double departureDistanceFromSun = arma::norm(departurePosition);
      double arrivalDistanceFromSun = arma::norm(arrivalPosition);     
      double depatureArrivalDotProduct = arma::dot(departurePosition, arrivalPosition);

      double t_m = 1.0;
      arma::uword maximalNumberOfRevolutions = 20;
      arma::uword numberOfRevolutions; 
      
      double A = 0.0;
      double B = 0.0;
      
      auto timeOfFlightFunction = [&A, &B, &t_m, &depatureArrivalDotProduct, &departureDistanceFromSun, &arrivalDistanceFromSun, &transferTime](
          const double parameter) {
        
        double gamma = depatureArrivalDotProduct / (departureDistanceFromSun * arrivalDistanceFromSun);
        A = t_m * std::sqrt(departureDistanceFromSun * arrivalDistanceFromSun * (1.0 + gamma));
        
        // Calculates the values for the second and third Stumpff function c2 and c3.
        double c2;
        double c3;
        if (parameter > 0){
          c2 = (1.0 - std::cos(std::sqrt(parameter))) / parameter;
          c3 = (1.0 - std::sin(std::sqrt(parameter)) / std::sqrt(parameter)) / parameter;
        } else if (parameter < 0) {
          c2 = (1.0 - std::cosh(std::sqrt(-parameter))) / parameter;
          c3 = (1.0 - std::sinh(std::sqrt(-parameter)) / std::sqrt(-parameter)) / parameter;
        } else {
          c2 = 1.0/2.0;
          c3 = 1.0/6.0;
        }
        
        B = departureDistanceFromSun + arrivalDistanceFromSun + 1.0 / std::sqrt(c2) * (A * (parameter * c3 - 1.0));
        double chi = std::sqrt(B / c2);
                 
        return 1.0 / std::sqrt(standardGravitationalParameterOfSun) * (std::pow(chi, 3.0) * c3 + A * std::sqrt(B)) / 86400.0 - transferTime;
      };
      
      double F;
      double G;
      double Gdot;
      
      auto calculateVelocityVectorsFunction = [&A, &B, &F, &G, &Gdot, &departurePosition, &departureDistanceFromSun, &arrivalPosition, &arrivalDistanceFromSun](){
        F = 1.0 - B / departureDistanceFromSun;
        G = A * std::sqrt(B / standardGravitationalParameterOfSun);
        Gdot = 1.0 - B / arrivalDistanceFromSun;
        
        return std::make_pair((1.0 / G) * (arrivalPosition - departurePosition * F), (1.0 / G) * (Gdot * arrivalPosition - departurePosition));
      };
      
      std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> velocityPairs;
      velocityPairs.reserve(maximalNumberOfRevolutions * 4 + 2);
      
      double lowerBound = -4.0 * arma::datum::pi;
      double upperBound = 4.0 * std::pow(arma::datum::pi, 2.0);
      
      // For zero revolutions
      double brentShortWaySolution = mant::brent(timeOfFlightFunction, lowerBound, upperBound, 100, 1e-10);
      velocityPairs.push_back(calculateVelocityVectorsFunction());
      t_m = -t_m;
      
      double brentLongWaySolution = mant::brent(timeOfFlightFunction, lowerBound, upperBound, 100, 1e-10);
      velocityPairs.push_back(calculateVelocityVectorsFunction());
      t_m = -t_m;
      
      mant::OptimisationProblem timeOfFlightMinimumProblem(1);
      timeOfFlightMinimumProblem.setObjectiveFunction([&timeOfFlightFunction](const arma::Col<double>& parameter){
        return timeOfFlightFunction(parameter(0));
      });
      
      mant::HookeJeevesAlgorithm timeOfFlightMinimumAlgorithm;
      timeOfFlightMinimumAlgorithm.setMaximalNumberOfIterations(100);

      for (numberOfRevolutions = 1; numberOfRevolutions <= maximalNumberOfRevolutions; numberOfRevolutions++) {
        
        lowerBound = 4.0 * std::pow((numberOfRevolutions) * arma::datum::pi, 2.0);
        upperBound = 4.0 * std::pow((numberOfRevolutions + 1) * arma::datum::pi, 2.0); 
        
        timeOfFlightMinimumProblem.setLowerBounds({lowerBound});
        timeOfFlightMinimumProblem.setUpperBounds({upperBound});          
        
        timeOfFlightMinimumAlgorithm.optimise(timeOfFlightMinimumProblem, arma::Col<double>((upperBound - lowerBound) / 2.0));          
        arma::Col<double> timeOfFlightMinimum = timeOfFlightMinimumAlgorithm.getBestParameter();

        double brentShortWaySolutionLeft = mant::brent(timeOfFlightFunction, lowerBound, timeOfFlightMinimum(0), 100, 1e-10);
        
        if(std::isnan(brentShortWaySolutionLeft)){
          break;
        }
        
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> brentShortWaySolutionLeftVelocities = calculateVelocityVectorsFunction();
        
        double brentShortWaySolutionRight = mant::brent(timeOfFlightFunction, timeOfFlightMinimum(0), upperBound, 100, 1e-10);
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> brentShortWaySolutionRightVelocities = calculateVelocityVectorsFunction();   
        t_m = -t_m;
        
        timeOfFlightMinimumAlgorithm.optimise(timeOfFlightMinimumProblem, arma::Col<double>((upperBound - lowerBound) / 2.0));          
        timeOfFlightMinimum = timeOfFlightMinimumAlgorithm.getBestParameter();
        
        double brentLongWaySolutionLeft = mant::brent(timeOfFlightFunction, lowerBound, timeOfFlightMinimum(0), 100, 1e-10);
        
        if(std::isnan(brentLongWaySolutionLeft)){
          break;
        }
        
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> brentLongWaySolutionLeftVelocities = calculateVelocityVectorsFunction();
        
        double brentLongWaySolutionRight = mant::brent(timeOfFlightFunction, timeOfFlightMinimum(0), upperBound, 100, 1e-10);
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> brentLongWaySolutionRightVelocities = calculateVelocityVectorsFunction();
        
        t_m = -t_m;
        
        if(std::isnan(brentShortWaySolutionLeft) || std::isnan(brentLongWaySolutionLeft)){
          break;
        }

        velocityPairs.push_back(brentShortWaySolutionLeftVelocities);          
        velocityPairs.push_back(brentShortWaySolutionRightVelocities);
        velocityPairs.push_back(brentLongWaySolutionLeftVelocities);
        velocityPairs.push_back(brentLongWaySolutionRightVelocities);
      }
      
      return velocityPairs;
    }
  }
}
