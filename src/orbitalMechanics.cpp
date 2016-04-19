#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/numericalAnalysis.hpp"

namespace mant {
  namespace itd {

    std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> orbitOnPosition(
        const double modifiedJulianDay2000,
        const arma::Col<double>::fixed<7>& keplerianElements) {
      verify(modifiedJulianDay2000 > -73048.0 && modifiedJulianDay2000 < 18263.0, "orbitOnPosition: The modifiedJulianDay2000 must be between -73048.0 and 18263.0.");

      double mjdReference = keplerianElements(6) - 51544.0; //mjd to mjd2000
      double nomalisedMjd2000 = (modifiedJulianDay2000 - mjdReference) * 86400.0; // in sec

      double semiMajorAxis = keplerianElements(0) * 149597870691.0; //in km
      double eccentricity = keplerianElements(1);
      double inclination = (arma::datum::pi / 180.0) * keplerianElements(2);
      double omg = (arma::datum::pi / 180.0) * keplerianElements(3);
      double omp = (arma::datum::pi / 180.0) * keplerianElements(4);
      double ea = (arma::datum::pi / 180.0) * keplerianElements(5);

      verify(eccentricity < 1.0, "orbitOnPosition: The eccentricity must be lesser than 1.0.");

      double meanMotion = std::sqrt(standardGravitationalParameterOfSun / std::pow(semiMajorAxis, 3.0));
      ea += nomalisedMjd2000 * meanMotion;

      //m2e begin
      double E = ea + eccentricity * std::cos(ea);
      ea = mant::brent([&ea, &eccentricity](
                           double parameter) { 
        return parameter - eccentricity * std::sin(parameter) - ea;
      },
          E - 1.0, E + 1.0, 100, 1e-10); //TODO bounds round about variable E, +- 1.0 okay? Variable E nesecary?
      //m2e end

      //par2ic begin
      double b;
      double n;
      double xper;
      double yper;
      double xdotper;
      double ydotper;

      b = semiMajorAxis * std::sqrt(1.0 - std::pow(eccentricity, 2.0));
      n = std::sqrt(standardGravitationalParameterOfSun / std::pow(semiMajorAxis, 3.0));
      xper = semiMajorAxis * (std::cos(ea) - eccentricity);
      yper = b * std::sin(ea);
      xdotper = -(semiMajorAxis * n * std::sin(ea)) / (1.0 - eccentricity * std::cos(ea));
      ydotper = (b * n * std::cos(ea)) / (1.0 - eccentricity * std::cos(ea));

      double cosomg = std::cos(omg);
      double cosomp = std::cos(omp);
      double sinomg = std::sin(omg);
      double sinomp = std::sin(omp);
      double cosi = std::cos(inclination);
      double sini = std::sin(inclination);

      arma::Mat<double>::fixed<3, 3> rotationMatrix = {{cosomg * cosomp - sinomg * sinomp * cosi, -cosomg * sinomp - sinomg * cosomp * cosi, sinomg * sini},
          {sinomg * cosomp + cosomg * sinomp * cosi, -sinomg * sinomp + cosomg * cosomp * cosi, -cosomg * sini},
          {sinomp * sini, cosomp * sini, cosi}};

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

    std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> orbitOnPosition(
        const double modifiedJulianDay2000,
        const arma::Mat<double>::fixed<2, 6>& keplerianElements) {
      verify(modifiedJulianDay2000 > -73048.0 && modifiedJulianDay2000 < 18263.0, "orbitOnPosition: The modifiedJulianDay2000 must be between -73048.0 and 18263.0.");

      // KeplerianElements should contain data from http://ssd.jpl.nasa.gov/txt/p_elem_t1.txt
      // Referenced date is 2010-01-01 12:00:00 which is equal to 0.5 in mjd2000 format
      double nomalisedMjd2000 = (modifiedJulianDay2000 - 0.5) / 36525.0; // in centrury

      arma::Row<double>::fixed<6> keplerValuesPreCalculationVector = keplerianElements.row(0) + keplerianElements.row(1) * nomalisedMjd2000;

      double semiMajorAxis = keplerValuesPreCalculationVector(0) * 149597870691.0; //in km
      double eccentricity = keplerValuesPreCalculationVector(1);
      double inclination = (arma::datum::pi / 180.0) * keplerValuesPreCalculationVector(2);
      double omg = (arma::datum::pi / 180.0) * keplerValuesPreCalculationVector(5);
      double omp = (arma::datum::pi / 180.0) * (keplerValuesPreCalculationVector(4) - keplerValuesPreCalculationVector(5));
      double ea = (arma::datum::pi / 180.0) * (keplerValuesPreCalculationVector(3) - keplerValuesPreCalculationVector(4));

      verify(eccentricity < 1.0, "orbitOnPosition: The eccentricity must be lesser than 1.0.");

      //m2e begin
      double E = ea + eccentricity * std::cos(ea);
      ea = mant::brent([&ea, &eccentricity](
                           double parameter) { 
        return parameter - eccentricity * std::sin(parameter) - ea;
      },
          E - 1.0, E + 1.0, 100, 1e-10); //TODO bounds round about variable E, +- 1.0 okay? Variable E nesecary?
      //m2e end

      //par2ic begin
      double b;
      double n;
      double xper;
      double yper;
      double xdotper;
      double ydotper;

      b = semiMajorAxis * std::sqrt(1.0 - std::pow(eccentricity, 2.0));
      n = std::sqrt(standardGravitationalParameterOfSun / std::pow(semiMajorAxis, 3.0));
      xper = semiMajorAxis * (std::cos(ea) - eccentricity);
      yper = b * std::sin(ea);
      xdotper = -(semiMajorAxis * n * std::sin(ea)) / (1.0 - eccentricity * std::cos(ea));
      ydotper = (b * n * std::cos(ea)) / (1.0 - eccentricity * std::cos(ea));

      double cosomg = std::cos(omg);
      double cosomp = std::cos(omp);
      double sinomg = std::sin(omg);
      double sinomp = std::sin(omp);
      double cosi = std::cos(inclination);
      double sini = std::sin(inclination);

      arma::Mat<double>::fixed<3, 3> rotationMatrix = {{cosomg * cosomp - sinomg * sinomp * cosi, -cosomg * sinomp - sinomg * cosomp * cosi, sinomg * sini},
          {sinomg * cosomp + cosomg * sinomp * cosi, -sinomg * sinomp + cosomg * cosomp * cosi, -cosomg * sini},
          {sinomp * sini, cosomp * sini, cosi}};

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
  }
}
