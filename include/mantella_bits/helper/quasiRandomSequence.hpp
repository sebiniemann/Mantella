#pragma once

// Armadillo
#include <armadillo>

namespace mant {
   arma::Mat<double> getHaltonSequence(
       const arma::Col<unsigned int>& seed,
       const arma::Col<unsigned int>& base,
       const unsigned int& numberOfColumms) noexcept;

   arma::Col<double> getVanDerCorputSequence(
       const unsigned int& seed,
       const unsigned int& base,
       const unsigned int& numberOfColumms) noexcept;
}
