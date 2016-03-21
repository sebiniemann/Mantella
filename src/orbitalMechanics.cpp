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

      double semiPerimeter = (flightDistance + departureDistanceFromSun + arrivalDistanceFromSun) / 2.0;

      arma::Col<double>::fixed<3> departurePositionNormalised = arma::normalise(departurePosition);
      arma::Col<double>::fixed<3> arrivalPositionNormalised = arma::normalise(arrivalPosition);

      arma::Col<double>::fixed<3> depatureArrivalCrossProduct = arma::normalise(arma::cross(departurePositionNormalised, arrivalPositionNormalised));

      verify(depatureArrivalCrossProduct(2) != 0.0, "lambert: depatureArrivalCrossProduct must have a z component unequal zero.");

      double lambda = std::sqrt(1.0 - flightDistance / semiPerimeter);

      if (depatureArrivalCrossProduct(2) < 0.0) {
        lambda = -lambda;
      }

      double T00 = std::acos(lambda) + lambda * sqrt(1.0 - std::pow(lambda, 2.0));

      arma::uword maximalNumberOfRevolutions = 20;
      arma::uword numberOfRevolutions; // = std::floor(T / arma::datum::pi);

      auto timeOfFlightFunction = [&lambda, &numberOfRevolutions](
          const double parameter) {
        double timeOfFlight;    
        if(std::abs(1.0 - parameter) < 1e-12) {
          return arma::datum::nan;
        } else {
          double a = 1.0 / (1.0 - std::pow(parameter, 2.0));
          timeOfFlight = std::abs(a) * std::sqrt(std::abs(a));
          
          if(parameter < 1) {
            double alpha = 2 * std::acos(parameter);
            double beta = std::copysign(2 * std::asin( std::sqrt( std::pow(lambda, 2.0) / a)), lambda);

            timeOfFlight *= (alpha - std::sin(alpha)) - (beta - std::sin(beta)) + 2.0 * arma::datum::pi * numberOfRevolutions;
          } else {
            double alpha = 2.0 * std::acosh(parameter);
            double beta = std::copysign(2.0 * std::asinh( std::sqrt( std::pow(lambda, 2.0) / -a)), lambda);
            
            timeOfFlight *= (beta - std::sinh(beta)) - (alpha - std::sinh(alpha));
          }
        }

        return timeOfFlight / 2.0;
      };

      std::vector<double> brentVector;
      if (T00 > 0.0) { //if (T < T0) { // Halley iterations to find xM and TM

        brentVector.reserve(maximalNumberOfRevolutions * 2);

        for (numberOfRevolutions = 0; numberOfRevolutions < maximalNumberOfRevolutions; numberOfRevolutions++) {
          brentVector.at(2 * numberOfRevolutions) = mant::brent(timeOfFlightFunction, -4.0 * arma::datum::pi, 4.0 * std::pow(arma::datum::pi, 2.0), 100, 1e-10);
          lambda = -lambda;

          brentVector.at(2 * numberOfRevolutions + 1) = mant::brent(timeOfFlightFunction, -4.0 * arma::datum::pi, 4.0 * std::pow(arma::datum::pi, 2.0), 100, 1e-10);
          lambda = -lambda;
        }
      }

      std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> velocitiesVector;
      velocitiesVector.reserve(maximalNumberOfRevolutions * 2 + 1);
      std::vector<double> xVector;
      xVector.reserve(maximalNumberOfRevolutions * 2 + 1);

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

      for (size_t i = 0; i < maximalNumberOfRevolutions; ++i) {
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

        velocitiesVector.push_back(std::make_pair(vr1 * departurePositionNormalised + vt1 * it1, vr2 * arrivalPositionNormalised + vt2 * it2));
      }

      return velocitiesVector;
    }
  }
}
