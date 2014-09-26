#include <hop_bits/optimisationProblem/benchmark/bbob2013/linearSlope.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  void LinearSlope::setOne(const arma::Col<double>& one) {
    one_ = one;
    xOpt_ = 5.0 * one_;
    scaling_ = arma::sign(one_) % getScaling(10);
    partiallyObjectiveValue_ = 5.0 * arma::accu(arma::abs(scaling_));
  }

  double LinearSlope::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z(parameter);

    arma::Col<arma::uword> outOfBound = arma::find(xOpt_ % z > 25.0);
    z.elem(outOfBound) %= xOpt_.elem(outOfBound);

    return partiallyObjectiveValue_ - arma::dot(scaling_, z);
  }

  std::string LinearSlope::to_string() const {
    return "LinearSlope";
  }
}

CEREAL_REGISTER_TYPE(hop::LinearSlope)
