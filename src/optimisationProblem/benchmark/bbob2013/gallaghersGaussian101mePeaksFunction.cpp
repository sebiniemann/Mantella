#include <hop_bits/optimisationProblem/benchmark/bbob2013/gallaghersGaussian101mePeaksFunction.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction(const unsigned int& numberOfDimensions)
    : BlackBoxOptimisationBenchmark2013(numberOfDimensions),
      weight_(101) {
    weight_.at(0) = 10.0;
    for(std::size_t n = 1; n < weight_.n_elem; ++n) {
      weight_.at(n) = 1.1 + 8.0 * static_cast<double>(n - 1) / 99.0;
    }
  }

  double GallaghersGaussian101mePeaksFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    double maximalValue = std::numeric_limits<double>::lowest();
    for(unsigned int k = 0; k < 101; ++k) {
      arma::Col<double> parameterTransposition = parameter - localOptimaY101_.col(k);
      maximalValue = std::max(maximalValue, weight_.at(k) * std::exp(-1.0 / (2.0 * static_cast<double>(numberOfDimensions_)) * dot(parameterTransposition, rotationR_.t() * arma::diagmat(deltaC101_.col(k)) * rotationR_ * parameterTransposition)));
    }

    return std::pow(getOscillationTransformation(10.0 - maximalValue), 2) + getPenality(parameter);
  }

  std::string GallaghersGaussian101mePeaksFunction::to_string() const {
    return "GallaghersGaussian101mePeaksFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::GallaghersGaussian101mePeaksFunction)
