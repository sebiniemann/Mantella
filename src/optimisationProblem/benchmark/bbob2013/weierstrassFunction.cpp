#include <hop_bits/optimisationProblem/benchmark/bbob2013/weierstrassFunction.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  WeierstrassFunction::WeierstrassFunction(const unsigned int& numberOfDimensions)
    : BlackBoxOptimisationBenchmark2013(numberOfDimensions) {
    f0_ = 0.0;
    for(unsigned int k = 0; k < 12; ++k) {
      f0_ += std::pow(0.5, k) * cos(2.0 * arma::datum::pi * pow(3, k) * 0.5);
    }
  }

  double WeierstrassFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = rotationR_ * (delta_ % (rotationQ_ * getOscillationTransformation(rotationR_ * (parameter - translation_))));

    double sum = 0;
    for (std::size_t n = 0; n < parameter.n_elem; ++n) {
      for (unsigned int k = 0; k < 12; ++k) {
        sum += std::pow(0.5, k) * std::cos(2.0 * arma::datum::pi * std::pow(3.0, k) * (z.at(n) + 0.5));
      }
    }

    return 10 * (std::pow(sum / static_cast<double>(numberOfDimensions_) - f0_, 3) + getPenality(parameter) / static_cast<double>(numberOfDimensions_));
  }

  std::string WeierstrassFunction::to_string() const {
    return "WeierstrassFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::WeierstrassFunction)
