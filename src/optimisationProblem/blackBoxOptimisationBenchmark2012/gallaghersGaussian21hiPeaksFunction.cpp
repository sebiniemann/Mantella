#include <hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/gallaghersGaussian21hiPeaksFunction.hpp>

// C++ Standard Library
#include <algorithm>
#include <cmath>
#include <limits>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2012 {
    GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2012(numberOfDimensions),
        weight_(21) {
      weight_.at(0) = 10.0;
      for (std::size_t n = 1; n < weight_.n_elem; ++n) {
        weight_.at(n) = 1.1 + 8.0 * static_cast<double>(n - 1) / 19.0;
      }
    }

    double GallaghersGaussian21hiPeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double maximalValue = std::numeric_limits<double>::lowest();
      for (unsigned int k = 0; k < 21; ++k) {
        const arma::Col<double>& parameterTransposition = parameter - localOptimaY21_.col(k);
        maximalValue = std::max(maximalValue, weight_.at(k) * std::exp(-1.0 / (2.0 * static_cast<double>(numberOfDimensions_)) * arma::dot(parameterTransposition, rotationR_.t() * arma::diagmat(deltaC21_.col(k)) * rotationR_ * parameterTransposition)));
      }

      return std::pow(getOscillationTransformation(10.0 - maximalValue), 2.0) + getPenality(parameter);
    }

    std::string GallaghersGaussian21hiPeaksFunction::to_string() const noexcept {
      return "GallaghersGaussian21hiPeaksFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2012::GallaghersGaussian21hiPeaksFunction)
