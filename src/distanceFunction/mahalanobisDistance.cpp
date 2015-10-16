#include <mantella_bits/distanceFunction/mahalanobisDistance.hpp>

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  MahalanobisDistance::MahalanobisDistance(
      const arma::Mat<double>& covariance)
    : DistanceFunction(covariance.n_rows),
      covariance_(covariance) {
    
  }

  double MahalanobisDistance::getLengthImplementation(
      const arma::Col<double>& parameter) const {
    return std::sqrt(arma::dot(parameter, arma::solve(covariance_, parameter)));
  }
  
  std::string MahalanobisDistance::toString() const {
    return "mahalanobis_distance";
  }
}
