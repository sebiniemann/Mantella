#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/numericalAnalysis.hpp"

namespace mant {
  namespace itd {

    std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> orbitOnPosition(
        const double modifiedJulianDay,
        const arma::Col<double>::fixed<7>& keplerianElements) {
      if (!std::isfinite(modifiedJulianDay)) {
        // TODO
        verify(std::isfinite(modifiedJulianDay), "orbitOnPosition: The modifiedJulianDay must be finite.");
      }

      double timePassedSincePeriapsis = (modifiedJulianDay - keplerianElements(6)) * 86400.0; // in sec

      double semiMajorAxis = keplerianElements(0) * 149597870691.0; //in km
      double eccentricity = keplerianElements(1);
      double inclination = (arma::datum::pi / 180.0) * keplerianElements(2);
      double omg = (arma::datum::pi / 180.0) * keplerianElements(3);
      double omp = (arma::datum::pi / 180.0) * keplerianElements(4);
      double ea = (arma::datum::pi / 180.0) * keplerianElements(5);

      if (eccentricity >= 1.0) {
        // TODO
        verify(eccentricity < 1.0, "orbitOnPosition: The eccentricity must be lesser than 1.0.");
      }
      double meanMotion = std::sqrt(standardGravitationalParameterOfSun / std::pow(semiMajorAxis, 3.0));
      ea += timePassedSincePeriapsis * meanMotion;

      //m2e begin
      ea = mant::brent([&ea, &eccentricity](
                           double parameter) { 
        return parameter - eccentricity * std::sin(parameter) - ea;
      },
          ea + eccentricity * std::cos(ea) - 1.0, ea + eccentricity * std::cos(ea) + 1.0, 100, 1e-10); //TODO bounds round about (ea + eccentricity * std::cos(ea)) +- 1.0 okay?
      //m2e end

      //par2ic begin
      double b = semiMajorAxis * std::sqrt(1.0 - std::pow(eccentricity, 2.0));
      double n = std::sqrt(standardGravitationalParameterOfSun / std::pow(semiMajorAxis, 3.0));
      double xper = semiMajorAxis * (std::cos(ea) - eccentricity);
      double yper = b * std::sin(ea);
      double xdotper = -(semiMajorAxis * n * std::sin(ea)) / (1.0 - eccentricity * std::cos(ea));
      double ydotper = (b * n * std::cos(ea)) / (1.0 - eccentricity * std::cos(ea));

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

      arma::Col<double>::fixed<3> positionVector = rotationMatrix * temp;
      arma::Col<double>::fixed<3> velocityVector = rotationMatrix * temp2;

      return std::make_pair(positionVector, velocityVector);
      //par2ic end
    }
  }
}
