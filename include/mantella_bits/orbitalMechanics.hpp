#pragma once

// C++ standard library
#include <utility>
#include <vector>

// Armadillo
#include <armadillo>

namespace mant {
  namespace itd {

    // from pykep:
    // ref_mjd2000(epoch(2451545.0,epoch::JD).mjd2000()
    //inline double jd2mjd2000(const double & in){
    //return ( in - 2451544.5 );
    //}
    // => 2451545.0 - 2451544.5
    // NEED???
    const double refModifiedJulianDay2000 = 0.5;
    const double standardGravitationalParameterOfSun = 1.32712440018e20;

    std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> lambert(
        const arma::Col<double>::fixed<3>& departurePosition,
        const arma::Col<double>::fixed<3>& arrivalPosition,
        const double transferTime,
        const bool isClockwise);

    arma::Col<double>::fixed<3> dTdx( //TODO remove when brent algorithm
        double DT,
        double DDT,
        double DDDT,
        const double x,
        const double T);

    std::pair<double, double> gravityAssist(
        const arma::Col<double>::fixed<3>& inboundVelocity,
        const arma::Col<double>::fixed<3>& outboundVelocity);

    std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> orbitOnPosition(
        const double modifiedJulianDay2000,
        const arma::Mat<double>::fixed<2, 6>& keplerianElements);

    double x2tof(
        double timeOfFlight,
        const double x,
        const double N,
        const double lambda);
  }
}
