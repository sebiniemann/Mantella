#include "mantella_bits/optimisationProblem/surrogateModel/radialBasisFunctionModel.hpp"

// Mantella
#include "mantella_bits/radialBasisFunction.hpp"

namespace mant {
  RadialBasisFunctionModel::RadialBasisFunctionModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::shared_ptr<RadialBasisFunction> radialBasisFunction)
      : SurrogateModel(samples),
        coefficients_(samples_.size()) {
  }

  void RadialBasisFunctionModel::modelImplementation() {
    arma::Mat<double> Phi(samples_.size() + numberOfDimensions_ + 1, samples_.size() + numberOfDimensions_ + 1, arma::fill::zeros);

    arma::Col<double> F(samples_.size() + numberOfDimensions_ + 1);
    F.tail(numberOfDimensions_).zeros();

    arma::uword i = 0;
    for (auto n = samples_.cbegin(); n != samples_.cend();) {
      const arma::Col<double>& parameter = n->first;
      const double objectiveValue = n->second;

      arma::uword j = 0;
      for (auto k = ++n; k != samples_.cend(); ++k) {
        Phi(i, j) = radialBasisFunction_(arma::norm(k->first - parameter));
        ++j;
      }

      Phi.submat(i, samples_.size(), i, samples_.size() + numberOfDimensions_) = parameter;

      F(i) = objectiveValue;
      ++i;
    }

    const arma::Col<double>& solution = arma::solve(Phi, F);
    coefficients_ = solution.head(samples_.size());
  }

  double RadialBasisFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    double objectiveValue = 0;

    arma::uword n = 0;
    for (const auto& sample : samples_) {
      objectiveValue += coefficients_(n) * radialBasisFunction_(arma::norm(parameter - sample.first));
      ++n;
    }

    return objectiveValue;
  }
}
