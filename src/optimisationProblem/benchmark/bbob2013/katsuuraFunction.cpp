#include <hop_bits/optimisationProblem/benchmark/bbob2013/katsuuraFunction.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double KatsuuraFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = rotationQ_ * (delta_ % (rotationR_ * (parameter - translation_)));

    double product = 1.0;
    for (std::size_t n = 0; n < z.n_elem; ++n) {
        double value = z.at(n);

        double sum = 0.0;
        for (unsigned int k = 1; k < 33; ++k) {
            double power = std::pow(2.0, k);
            sum += std::abs(power * value - std::round(power * value)) / power;
        }

        product *= std::pow(1.0 + static_cast<double>(n + 1) * sum, (10.0 / std::pow(numberOfDimensions_, 1.2)));
    }

    return 10.0 / std::pow(numberOfDimensions_, 2) * (product - 1.0) + getPenality(parameter);
  }

  std::string KatsuuraFunction::to_string() const {
    return "KatsuuraFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::KatsuuraFunction)
