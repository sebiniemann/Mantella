#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <cmath>
#include <iostream>

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
      
      std::cout << "START LAMBERT" << std::endl;

      double departureDistanceFromSun = arma::norm(departurePosition);
      double arrivalDistanceFromSun = arma::norm(arrivalPosition);     
      double depatureArrivalDotProduct = arma::dot(departurePosition, arrivalPosition);
      
      arma::uword maximalNumberOfRevolutions = 0;
      arma::uword numberOfRevolutions; 
      
      double t_m = 1.0;
      double A = t_m * std::sqrt(departureDistanceFromSun * arrivalDistanceFromSun * (1.0 + depatureArrivalDotProduct / (departureDistanceFromSun * arrivalDistanceFromSun)));
      double B = 0.0;
      
      auto timeOfFlightFunction = [&A, &B, &t_m, &departureDistanceFromSun, &arrivalDistanceFromSun, &transferTime](
          const double parameter) {
                
        A = t_m * A;
        std::cout << "A: " << A << "  -  ";
                
        // Calculates the values for the second and third Stumpff function c2 and c3.
        double c2;
        double c3;
        if (parameter > 0) {
          c2 = (1.0 - std::cos(std::sqrt(parameter))) / parameter;
          c3 = (1.0 - std::sin(std::sqrt(parameter)) / std::sqrt(parameter)) / parameter;
        } else if (parameter < 0) {
          c2 = (1.0 - std::cosh(std::sqrt(-parameter))) / parameter;
          c3 = (1.0 - std::sinh(std::sqrt(-parameter)) / std::sqrt(-parameter)) / parameter;
        } else {
          c2 = 1.0/2.0;
          c3 = 1.0/6.0;
        }
        
        std::cout << "c2: " << c2 << "  -  ";
        std::cout << "c3: " << c3 << "  -  ";
        
        B = departureDistanceFromSun + arrivalDistanceFromSun + 1.0 / std::sqrt(c2) * (A * (parameter * c3 - 1.0));
        
        std::cout << "B: " << B << "  -  ";
        std::cout << "val: " << 1.0 / std::sqrt(standardGravitationalParameterOfSun) * (std::pow(std::sqrt(B / c2), 3.0) * c3 + A * std::sqrt(B)) / 86400.0 - transferTime << std::endl;
                 
        return (1.0 / std::sqrt(standardGravitationalParameterOfSun)) * (std::pow(std::sqrt(B / c2), 3.0) * c3 + A * std::sqrt(B)) / 86400.0 - transferTime;
      };
      
      std::cout << "function: " << timeOfFlightFunction(3.0 * std::pow(arma::datum::pi, 2.0)) << std::endl;
      
      auto calculateVelocityVectorsFunction = [&A, &B, &departurePosition, &departureDistanceFromSun, &arrivalPosition, &arrivalDistanceFromSun]() -> std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>{
        std::cout << "begin calculateVelocityVectorsFunction" << std::endl;
        double F = 1.0 - B / departureDistanceFromSun;
        double G = A * std::sqrt(B / standardGravitationalParameterOfSun);
        double Gdot = 1.0 - B / arrivalDistanceFromSun;
        std::cout << "end calculateVelocityVectorsFunction" << std::endl;
        return {(1.0 / G) * (arrivalPosition - departurePosition * F), (1.0 / G) * (Gdot * arrivalPosition - departurePosition)};
      };
      std::cout << "pre decl" << std::endl;
      std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> velocityPairs;
      velocityPairs.reserve(maximalNumberOfRevolutions * 4 + 2);
      std::cout << "post decl" << std::endl;
      double lowerBound = -2.0 * arma::datum::pi;
      double upperBound = 3.0 * std::pow(arma::datum::pi, 2.0);
      
      // For zero revolutions
      std::cout << "0" << std::endl;
      double brentShortWaySolution = mant::brent(timeOfFlightFunction, lowerBound, upperBound, 100, 1e-10);
      std::cout << "pre push_back" << std::endl;
      //std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> varr = calculateVelocityVectorsFunction();
      //std::cout << "varr type: " << typeid(varr).name() << std::endl;
      velocityPairs.push_back(calculateVelocityVectorsFunction());
      std::cout << "post push_back" << std::endl;
      t_m = -t_m;
      std::cout << "1" << std::endl;
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
        
        lowerBound = 6.0 * std::pow((numberOfRevolutions) * arma::datum::pi, 2.0);
        upperBound = 2.0 * std::pow((numberOfRevolutions + 1) * arma::datum::pi, 2.0); 
        std::cout << "(lower, upper) = (" << lowerBound << ", " << upperBound << ")" << std::endl;
        timeOfFlightMinimumProblem.setLowerBounds({lowerBound});
        timeOfFlightMinimumProblem.setUpperBounds({upperBound});          
        
      timeOfFlightMinimumAlgorithm.optimise(timeOfFlightMinimumProblem, {(upperBound - lowerBound) / 2.0});          
        arma::Col<double> timeOfFlightMinimum = timeOfFlightMinimumAlgorithm.getBestParameter();
        std::cout << "2 - " << numberOfRevolutions << std::endl;
        double brentShortWaySolutionLeft = mant::brent(timeOfFlightFunction, lowerBound, timeOfFlightMinimum(0), 100, 1e-10);
        
        if(std::isnan(brentShortWaySolutionLeft)){
          break;
        }

        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> brentShortWaySolutionLeftVelocities = calculateVelocityVectorsFunction();
        std::cout << "3 - " << numberOfRevolutions << std::endl;
        double brentShortWaySolutionRight = mant::brent(timeOfFlightFunction, timeOfFlightMinimum(0), upperBound, 100, 1e-10);
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> brentShortWaySolutionRightVelocities = calculateVelocityVectorsFunction();   
        t_m = -t_m;
        
        timeOfFlightMinimumAlgorithm.optimise(timeOfFlightMinimumProblem, {(upperBound - lowerBound) / 2.0});          
        timeOfFlightMinimum = timeOfFlightMinimumAlgorithm.getBestParameter();
        std::cout << "4 - " << numberOfRevolutions << std::endl;
        double brentLongWaySolutionLeft = mant::brent(timeOfFlightFunction, lowerBound, timeOfFlightMinimum(0), 100, 1e-10);
        
        if(std::isnan(brentLongWaySolutionLeft)){
          break;
        }
        
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> brentLongWaySolutionLeftVelocities = calculateVelocityVectorsFunction();
        std::cout << "5 - " << numberOfRevolutions << std::endl;
        double brentLongWaySolutionRight = mant::brent(timeOfFlightFunction, timeOfFlightMinimum(0), upperBound, 100, 1e-10);
        std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> brentLongWaySolutionRightVelocities = calculateVelocityVectorsFunction();
        
        t_m = -t_m;

        velocityPairs.push_back(brentShortWaySolutionLeftVelocities);          
        velocityPairs.push_back(brentShortWaySolutionRightVelocities);
        velocityPairs.push_back(brentLongWaySolutionLeftVelocities);
        velocityPairs.push_back(brentLongWaySolutionRightVelocities);
      }
      
      return velocityPairs;
    }
  }
}
