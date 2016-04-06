#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/numericalAnalysis.hpp"

namespace mant {
  namespace itd {

    std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> lambert(
        const arma::Col<double>::fixed<3>& departurePosition,
        const arma::Col<double>::fixed<3>& arrivalPosition,
        const double transferTime) {
      verify(transferTime > 0.0, "lambert: The transfer time must be greater than zero.");

      // 1 - Getting lambda and Ts
      double flightDistance = arma::norm(departurePosition - arrivalPosition);

      double departureDistanceFromSun = arma::norm(departurePosition);
      double arrivalDistanceFromSun = arma::norm(arrivalPosition);
      
      double depatureArrivalDotProduct = arma::dot(departurePosition, arrivalPosition);

      double semiPerimeter = (flightDistance + departureDistanceFromSun + arrivalDistanceFromSun) / 2.0;

      arma::Col<double>::fixed<3> departurePositionNormalised = arma::normalise(departurePosition);
      arma::Col<double>::fixed<3> arrivalPositionNormalised = arma::normalise(arrivalPosition);

      arma::Col<double>::fixed<3> depatureArrivalCrossProduct = arma::normalise(arma::cross(departurePositionNormalised, arrivalPositionNormalised));

      verify(depatureArrivalCrossProduct(2) != 0.0, "lambert: depatureArrivalCrossProduct must have a z component unequal zero.");

      double lambda = std::sqrt(1.0 - flightDistance / semiPerimeter);

      if (depatureArrivalCrossProduct(2) < 0.0) {
        lambda = -lambda;
      }

      //double T00 = std::acos(lambda) + lambda * sqrt(1.0 - std::pow(lambda, 2.0));

      arma::uword maximalNumberOfRevolutions = 20;
      arma::uword numberOfRevolutions; // = std::floor(T / arma::datum::pi);
      std::cout << "0" << std::endl;
      auto timeOfFlightFunction = [&depatureArrivalDotProduct, &departureDistanceFromSun, &arrivalDistanceFromSun, &transferTime, &standardGravitationalParameterOfSun](
          const double parameter) {
        
        double gamma = depatureArrivalDotProduct / (departureDistanceFromSun * arrivalDistanceFromSun);
        double A = std::sqrt(departureDistanceFromSun * arrivalDistanceFromSun * (1.0 + gamma));
        
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
        
        double B = departureDistanceFromSun + arrivalDistanceFromSun + 1.0 / std::sqrt(c2) * (A * (parameter * c3 - 1.0));
        double chi = std::sqrt(B / c2);
            
       
        return 1.0 / std::sqrt(standardGravitationalParameterOfSun) * (std::pow(chi, 3.0) * c3 + A * std::sqrt(B)) / 86400.0; - transferTime;
      };
      std::cout << "00" << std::endl;
      std::vector<double> brentVector;
      if (std::acos(lambda) + lambda * sqrt(1.0 - std::pow(lambda, 2.0)) > 0.0) { //if (T < T0) { // Halley iterations to find xM and TM

        brentVector.reserve(maximalNumberOfRevolutions * 2);
        
        double lowerBound = -4.0 * arma::datum::pi;
        double upperBound = 4.0 * std::pow(arma::datum::pi, 2.0);

        for (numberOfRevolutions = 0; numberOfRevolutions < maximalNumberOfRevolutions; numberOfRevolutions++) {
          
          std::cout << "00pre.numberOfRevolutions = " << numberOfRevolutions << std::endl;
          
          brentVector.push_back(mant::brent(timeOfFlightFunction, lowerBound, upperBound, 100, 1e-10));
          lambda = -lambda;

          std::cout << "00mid.numberOfRevolutions = " << numberOfRevolutions << std::endl;
          
          brentVector.push_back(mant::brent(timeOfFlightFunction, lowerBound, upperBound, 100, 1e-10));
          lambda = -lambda;
          
          std::cout << "00post.numberOfRevolutions = " << numberOfRevolutions << std::endl;

          lowerBound = 4.0 * std::pow((numberOfRevolutions + 1) * arma::datum::pi, 2.0);
          upperBound = 4.0 * std::pow((numberOfRevolutions + 2) * arma::datum::pi, 2.0);
        }
      }
      std::cout << "000" << std::endl;
      std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> velocitiesVector;
      velocitiesVector.reserve(maximalNumberOfRevolutions * 2);
      std::cout << "0000" << std::endl;
      double gamma = std::sqrt(standardGravitationalParameterOfSun * semiPerimeter / 2.0);
      double rho = (departureDistanceFromSun - arrivalDistanceFromSun) / flightDistance;
      double sigma = std::sqrt(1 - std::pow(rho, 2.0));

      double vt;
      double vr1;
      double vt1;
      double vr2;
      double vt2;
      double y;

      arma::Col<double>::fixed<3> it1;
      arma::Col<double>::fixed<3> it2;
      if (depatureArrivalCrossProduct(2) < 0.0) {
        it1 = arma::normalise(arma::cross(departurePositionNormalised, depatureArrivalCrossProduct));
        it2 = arma::normalise(arma::cross(arrivalPositionNormalised, depatureArrivalCrossProduct));
      } else {
        it1 = arma::normalise(arma::cross(depatureArrivalCrossProduct, departurePositionNormalised));
        it2 = arma::normalise(arma::cross(depatureArrivalCrossProduct, arrivalPositionNormalised));
      }

      //auto velocityVectorsFunction = [&lambda, &i](){};
      std::cout << "1" << std::endl;
      for (arma::uword i = 0; i < maximalNumberOfRevolutions; ++i) {
        y = std::sqrt(std::pow(lambda, 2.0) * (std::pow(brentVector.at(2 * i), 2.0) - 1.0) + 1.0);
        vr1 = gamma * ((lambda * y - brentVector.at(2 * i)) - rho * (lambda * y + brentVector.at(2 * i))) / departureDistanceFromSun;
        vr2 = -gamma * ((lambda * y - brentVector.at(2 * i)) + rho * (lambda * y + brentVector.at(2 * i))) / arrivalDistanceFromSun;

        vt = gamma * sigma * (y + lambda * brentVector.at(2 * i));
        vt1 = vt / departureDistanceFromSun;
        vt2 = vt / arrivalDistanceFromSun;

        velocitiesVector.push_back(std::make_pair(vr1 * departurePositionNormalised + vt1 * it1, vr2 * arrivalPositionNormalised + vt2 * it2));

        it1 = -it1;
        it2 = -it2;
        lambda = -lambda;

        y = std::sqrt(std::pow(lambda, 2.0) * (std::pow(brentVector.at(2 * i + 1), 2.0) - 1.0) + 1.0);
        vr1 = gamma * ((lambda * y - brentVector.at(2 * i + 1)) - rho * (lambda * y + brentVector.at(2 * i + 1))) / departureDistanceFromSun;
        vr2 = -gamma * ((lambda * y - brentVector.at(2 * i + 1)) + rho * (lambda * y + brentVector.at(2 * i + 1))) / arrivalDistanceFromSun;

        vt = gamma * sigma * (y + lambda * brentVector.at(2 * i + 1));
        vt1 = vt / departureDistanceFromSun;
        vt2 = vt / arrivalDistanceFromSun;

        it1 = -it1;
        it2 = -it2;
        lambda = -lambda;
        std::cout << "1.1 - " << i << std::endl;
        velocitiesVector.push_back(std::make_pair(vr1 * departurePositionNormalised + vt1 * it1, vr2 * arrivalPositionNormalised + vt2 * it2));
      }
      std::cout << "2" << std::endl;
      return velocitiesVector;
    }
  }
}
