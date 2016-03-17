#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>
#include <iostream>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/numericalAnalysis.hpp"

namespace mant {
  namespace itd {

    std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> lambert(
        const arma::Col<double>::fixed<3>& departurePosition,
        const arma::Col<double>::fixed<3>& arrivalPosition,
        const double transferTime,
        const bool isClockwise) {
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

      if (isClockwise) {
        lambda = -lambda;
      }

      double T = std::sqrt(2.0 * standardGravitationalParameterOfSun / std::pow(semiPerimeter, 3.0)) * transferTime;

      // 2 - We now have lambda, T and we will find all x
      // 2.1 - Let us first detect the maximum number of revolutions for which there exists a solution
      double nMax = T / arma::datum::pi;
      double T00 = std::acos(lambda) + lambda * sqrt(1.0 - std::pow(lambda, 2.0));
      double T0 = T00 + T; //+ nMax * arma::datum::pi;
      double T1 = 2.0 / 3.0 * (1.0 - std::pow(lambda, 3.0));

      if (nMax > 0.0) {
        if (T00 > 0.0) { //if (T < T0) { // Halley iterations to find xM and TM
          double tNew = mant::brent(
              [&lambda, &nMax](const double parameter) {
            double umx2 = 1.0 - std::pow(parameter, 2.0);
            double y = std::sqrt(1.0 - std::pow(lambda, 2.0) * umx2);
            double timeOfFlight;
            
            if(std::abs(1.0 - parameter) < 1e-12) {
              timeOfFlight = arma::datum::nan;
            } else {
              double a = 1.0 / (1.0 - std::pow(parameter, 2.0));
              timeOfFlight = (std::abs(a) * std::sqrt(std::abs(a));
              
              if(parameter < 1) {
                alfa = 2 * std::acos(parameter);
                beta = std::copysign(2 * std::asin( std::sqrt( std::pow(lambda, 2.0) / a)), lambda);

                timeOfFlight *= (alfa - std::sin(alfa)) - (beta - std::sin(beta)) + 2.0 * arma:datum::pi * nMax;
              } else {
                alfa = 2.0 * std::acosh(parameter);
                beta = std::copysign(2.0 * std::asinh( std::sqrt( std::pow(lambda, 2.0) / -a)), lambda);
                
                timeOfFlight *= (beta - std::sinh(beta)) - (alfa - std::sinh(alfa));
              }
            }

            return 1.0 / umx2 * (3.0 * timeOfFlight * parameter - 2.0 + 2.0 * std::pow(lambda, 3.0) * parameter / y) / 2.0;
              },
              -4.0 * arma::datum::pi, 4.0 * std::pow(arma::datum::pi, 2.0), 100, 1e-10); //TODO bounds

          if (tNew > T) { // TODO: T0 ?
            nMax--;
          }
        }
      }
      
      
      ///////////////////////////
      
      
      
         //(a * std::sqrt(a) * ((alfa - std::sin(alfa)) + 2.0 * arma::datum::pi * N)) / 2.0;
         //-(a * std::sqrt(a) * ((alfa - std::sinh(alfa))) / 2.0;
      
      
      
      // if(std::abs(1.0 - x) < 1e-12) {
        // return arma::datum::nan;
      // } else {
        // double a = 1.0 / (1.0 - std::pow(x, 2.0));
        // timeOfFlight = (std::abs(a) * std::sqrt(std::abs(a));
        
        // if(x < 1) {
          // alfa = 2 * std::acos(x);
          // beta = std::copysign(2 * std::asin( std::sqrt( std::pow(lambda, 2.0) / a)), lambda);

          // timeOfFlight *= (alfa - std::sin(alfa)) - (beta - std::sin(beta)) + 2.0 * arma:datum::pi * N;
        // } else {
          // alfa = 2.0 * std::acosh(x);
          // beta = std::copysign(2.0 * std::asinh( std::sqrt( std::pow(lambda, 2.0) / -a)), lambda);
          
          // timeOfFlight *= (beta - std::sinh(beta)) - (alfa - std::sinh(alfa));
        // }
      // }
      
      // return timeOfFlight / 2.0; 
      
      
      ///////////////////////////

      // We exit this if clause with Mmax being the maximum number of revolutions
      // for which there exists a solution. We crop it to m_multi_revs
      nMax = std::min(20.0, nMax);

      // 2.2 We now allocate the memory for the output variables
      //std::vector<arma::Col<double>::fixed<3>> v1Vec; // std::vector< std::pair< arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3> > >
      //v1Vec.resize(nMax * 2 + 1);
      //std::vector<arma::Col<double>::fixed<3>> v2Vec;
      //v2Vec.resize(nMax * 2 + 1);
      std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> velocitiesVector;
      velocitiesVector.reserve((arma::uword)nMax * 2 + 1);
      //std::vector<int> itersVec;
      //itersVec.resize(nMax * 2 + 1);
      std::vector<double> xVector;
      xVector.reserve((arma::uword)nMax * 2 + 1);

      // 3 - We may now find all solutions in x,y
      // 3.1 0 rev solution
      // 3.1.1 initial guess
      if (T >= T00) {
        xVector.at(0) = -(T - T00) / (T - T00 + 4.0);
      } else if (T <= T1) {
        xVector.at(0) = T1 * (T1 - T) / (2.0 / 5.0 * (1.0 - std::pow(lambda, 5.0)) * T) + 1.0;
      } else {
        xVector.at(0) = std::pow((T / T00), 0.69314718055994529 / std::log(T1 / T00)) - 1.0;
      }

      // 3.1.2 Householder iterations
      //itersVec[0] = brent(...); //need??? or TODO -> householder(T, xVector[0], 0.0, 1e-5, 15);

      // 3.2 multi rev solutions
      double tmp;
      for (std::size_t i = 1; i < nMax + 1; ++i) {
        //3.2.1 left Householder iterations
        tmp = std::pow((i * arma::datum::pi + arma::datum::pi) / (8.0 * T), 2.0 / 3.0);
        xVector.at(2 * i - 1) = (tmp - 1.0) / (tmp + 1.0);
        //itersVec[2 * i - 1] = brent(...); //need??? or TODO -> householder(T, xVector[2*i-1], i, 1e-8, 15);

        //3.2.2 right Householder iterations
        tmp = std::pow((8.0 * T) / (i * arma::datum::pi), 2.0 / 3.0);
        xVector.at(2 * i) = (tmp - 1.0) / (tmp + 1.0);
        //itersVec[2 * i] = brent(...); //need??? or TODO -> householder(T, xVector[2 * i], i, 1e-8, 15);
      }

      // 4 - For each found x value we reconstruct the terminal velocities
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

      if (isClockwise) {
        it1 = -it1;
        it2 = -it2;
      }

      for (size_t i = 0; i < xVector.size(); ++i) {
        y = std::sqrt(std::pow(lambda, 2.0) * (std::pow(xVector.at(i), 2.0) - 1.0) + 1.0);
        vr1 = gamma * ((lambda * y - xVector.at(i)) - rho * (lambda * y + xVector.at(i))) / departureDistanceFromSun;
        vr2 = -gamma * ((lambda * y - xVector.at(i)) + rho * (lambda * y + xVector.at(i))) / arrivalDistanceFromSun;

        vt = gamma * sigma * (y + lambda * xVector.at(i));
        vt1 = vt / departureDistanceFromSun;
        vt2 = vt / arrivalDistanceFromSun;

        //v1Vec.at(i) = vr1 * departurePositionNormalised + vt1 * it1;
        //v2Vec.at(i) = vr2 * arrivalPositionNormalised + vt2 * it2;

        velocitiesVector.push_back(std::make_pair(vr1 * departurePositionNormalised + vt1 * it1, vr2 * arrivalPositionNormalised + vt2 * it2));
      }

      return velocitiesVector;
    }

    std::pair<double, double> gravityAssist(
        const arma::Col<double>::fixed<3>& inboundVelocity,
        const arma::Col<double>::fixed<3>& outboundVelocity) {
      double inboundVelocityLength = arma::norm(inboundVelocity);
      double outboundVelocityLength = arma::norm(outboundVelocity);

      double velocitiesDotProduct = arma::dot(inboundVelocity, outboundVelocity);
      double alpha = std::acos(velocitiesDotProduct / (inboundVelocityLength * outboundVelocityLength));

      double inboundAcceleration = 1.0 / std::pow(inboundVelocityLength, 2.0);
      double outboundAcceleration = 1.0 / std::pow(outboundVelocityLength, 2.0);

      double rp = mant::brent(
                [&inboundAcceleration, &outboundAcceleration, &alpha](double parameter) { 
              return std::asin(inboundAcceleration / (inboundAcceleration + parameter)) + std::asin(outboundAcceleration / (outboundAcceleration + parameter)) - alpha; 
                }, 
                -2.0 - alpha, 2.0 + alpha, 100, 1e-10); //TODO bounds

      double DV = std::abs(std::sqrt(std::pow(outboundVelocityLength, 2) + 2.0 / rp) - std::sqrt(std::pow(inboundVelocityLength, 2) + 2.0 / rp));

      //TODO correct?
      return std::make_pair(rp, DV);
    }

    std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> orbitOnPosition(
        const double modifiedJulianDay2000,
        const arma::Mat<double>::fixed<2, 6>& keplerianElements) {
      verify(modifiedJulianDay2000 > -73048.0 && modifiedJulianDay2000 < 18263.0, "orbitOnPosition: The modifiedJulianDay2000 must be between -73048.0 and 18263.0.");

      double nomalisedMjd2000 = modifiedJulianDay2000 - refModifiedJulianDay2000 / 36525.0;

      arma::Col<double>::fixed<6> keplerValuesPreCalculationVector = keplerianElements.col(0) + keplerianElements.col(1) * nomalisedMjd2000;

      double semiMajorAxis = keplerValuesPreCalculationVector(0) * 149597870691.0;
      double eccentricity = keplerValuesPreCalculationVector(1);
      double inclination = (arma::datum::pi / 180.0) * keplerValuesPreCalculationVector(2);
      double omg = (arma::datum::pi / 180.0) * keplerValuesPreCalculationVector(5);
      double omp = (arma::datum::pi / 180.0) * (keplerValuesPreCalculationVector(4) - keplerValuesPreCalculationVector(5));
      double ea = (arma::datum::pi / 180.0) * (keplerValuesPreCalculationVector(3) - keplerValuesPreCalculationVector(4));

      //m2e begin
      //double E = ea + eccentricity * std::cos(ea);
      ea = mant::brent(
          [&eccentricity, &ea](double parameter) { 
        return parameter - eccentricity * sin(parameter) - ea; 
          }, 
          0.0, 2.0 * arma::datum::pi + 10.0, 100, 1e-10); //TODO bounds round about variable E, +10.0 wrong? Variable E nesecary?
      //m2e end

      //par2ic begin
      double b;
      double n;
      double xper;
      double yper;
      double xdotper;
      double ydotper;
      double dNdZeta;

      if (eccentricity < 1.0) {
        b = semiMajorAxis * std::sqrt(1.0 - std::pow(eccentricity, 2.0));
        n = std::sqrt(standardGravitationalParameterOfSun / std::pow(semiMajorAxis, 3.0));

        xper = semiMajorAxis * (std::cos(ea) - eccentricity);
        yper = b * std::sin(ea);
        xdotper = -(semiMajorAxis * n * std::sin(ea)) / (1.0 - eccentricity * std::cos(ea));
        ydotper = (b * n * std::cos(ea)) / (1.0 - eccentricity * std::cos(ea));
      } else {
        b = -semiMajorAxis * std::sqrt(std::pow(eccentricity, 2.0) - 1.0);
        n = std::sqrt(-standardGravitationalParameterOfSun / std::pow(semiMajorAxis, 3.0));

        dNdZeta = eccentricity * (1.0 + std::pow(std::tan(ea), 2.0)) - (0.5 + 0.5 * std::pow(std::tan(0.5 * ea + (arma::datum::pi / 4.0)), 2)) / std::tan(0.5 * ea + (arma::datum::pi / 4.0));

        xper = semiMajorAxis / std::cos(ea) - semiMajorAxis * eccentricity;
        yper = b * std::tan(ea);
        xdotper = semiMajorAxis * std::tan(ea) / std::cos(ea) * n / dNdZeta;
        ydotper = b / std::pow(std::cos(ea), 2.0) * n / dNdZeta;
      }

      double cosomg = std::cos(omg);
      double cosomp = std::cos(omp);
      double sinomg = std::sin(omg);
      double sinomp = std::sin(omp);
      double cosi = std::cos(inclination);
      double sini = std::sin(inclination);

      arma::Mat<double>::fixed<3, 3> rotationMatrix;
      rotationMatrix(0, 0) = cosomg * cosomp - sinomg * sinomp * cosi;
      rotationMatrix(0, 1) = -cosomg * sinomp - sinomg * cosomp * cosi;
      rotationMatrix(0, 2) = sinomg * sini;
      rotationMatrix(1, 0) = sinomg * cosomp + cosomg * sinomp * cosi;
      rotationMatrix(1, 1) = -sinomg * sinomp + cosomg * cosomp * cosi;
      rotationMatrix(1, 2) = -cosomg * sini;
      rotationMatrix(2, 0) = sinomp * sini;
      rotationMatrix(2, 1) = cosomp * sini;
      rotationMatrix(2, 2) = cosi;

      arma::Col<double>::fixed<3> temp = {xper, yper, 0.0};
      arma::Col<double>::fixed<3> temp2 = {xdotper, ydotper, 0.0};

      arma::Col<double>::fixed<3> positionVector = {0.0, 0.0, 0.0};
      arma::Col<double>::fixed<3> velocityVector = {0.0, 0.0, 0.0};

      for (arma::uword j = 0; j < 3; ++j) {
        for (arma::uword k = 0; k < 3; ++k) {
          positionVector(j) += rotationMatrix(j, k) * temp(k);
          velocityVector(j) += rotationMatrix(j, k) * temp2(k);
        }
      }

      return std::make_pair(positionVector, velocityVector);
      //par2ic end
    }

    // double x2tof(
        // double timeOfFlight,
        // const double x,
        // const double N,
        // const double lambda) {
      // double battin = 0.01;
      // double lagrange = 0.2;
      // double dist = std::abs(x - 1.0);

      

      
      
      // if (dist < lagrange && dist > battin) {
        // double a = 1.0 / (1.0 - std::pow(x, 2.0));
        // if (a > 0) {
          // double alfa = 2.0 * std::acos(x);
          // double beta = 2.0 * std::asin(std::sqrt(std::pow(lambda, 2.0) / a));

          // if (lambda < 0.0) {
            // beta = -beta;
          // }

          
          // timeOfFlight = (a * std::sqrt(a) * ((alfa - std::sin(alfa)) - (beta - std::sin(beta)) + 2.0 * arma::datum::pi * N)) / 2.0;
        // } else {
          // double alfa = 2.0 * std::acosh(x);
          // double beta = 2.0 * std::asinh(std::sqrt(-std::pow(lambda, 2.0) / a));

          // if (lambda < 0.0) {
            // beta = -beta;
          // }

          // timeOfFlight = -a * std::sqrt(-a) * ((beta - std::sinh(beta)) - (alfa - std::sinh(alfa))) / 2.0;
        // }
      // } else {
        // double K = std::pow(lambda, 2.0);
        // double E = std::pow(x, 2.0) - 1.0;
        // double rho = std::abs(E);
        // double z = std::sqrt(1.0 + K * E);

        // if (dist < battin) { // We use Battin series timeOfFlight expression
          // double eta = z - lambda * x;
          // //double S1 = 0.5 * (1.0 - lambda - x * eta);

          // //hypergeometric function start
          // double Sj = 1.0;
          // double Cj = 1.0;
          // double Cj1 = 0.0;
          // double Sj1 = 0.0;

          // int shift = 0;
          // double err = 1.0;

          // while (err > 1e-11) { // && shift < XY  ??????
            // Cj1 = Cj * (3.0 + shift) * (1.0 + shift) / (2.5 + shift) * 0.5 * (1.0 - lambda - x * eta) / (shift + 1);
            // Sj1 = Sj + Cj1;
            // err = std::abs(Cj1);
            // Sj = Sj1;
            // Cj = Cj1;
            // shift++;
          // }
          // //hypergeometric function end

          // timeOfFlight = (std::pow(eta, 3.0) * (4.0 / 3.0) * Sj + 4.0 * lambda * eta) / 2.0 + N * arma::datum::pi / std::pow(rho, 1.5);

        // } else { // We use Lancaster timeOfFlight expresion

          // double y = std::sqrt(rho);
          // double g = x * z - lambda * E;
          // double d = 0.0;

          // if (E < 0) {
            // double l = std::acos(g);
            // d = N * arma::datum::pi + l;

          // } else {
            // double f = y * (z - lambda * x);
            // d = log(f + g);
          // }
          // timeOfFlight = (x - lambda * z - d / y) / E;
        // }
      // }
      // return timeOfFlight;
    // }
  }
}
