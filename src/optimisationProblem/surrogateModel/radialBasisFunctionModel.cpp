#include <mantella_bits/optimisationProblem/surrogateModel/radialBasisFunctionModel.hpp>

namespace mant {
  RadialBasisFunctionModel::RadialBasisFunctionModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::shared_ptr<RadialBasisFunction> radialBasisFunction)
    : SurrogateModel(samples),
      radialBasisFunction_(radialBasisFunction),
      coefficients_(samples_.size()),
      tail_(numberOfDimensions_ + 1) {
      
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
        Phi(i, j) = radialBasisFunction_->getBasisFunctionValue(arma::norm(k->first - parameter));
        ++j;
      }
      
      Phi.submat(i, samples_.size(), i, samples_.size() + numberOfDimensions_) = parameter;
      
      F(i) = objectiveValue;
      ++i;
    }
    
    const arma::Col<double>& solution = arma::solve(Phi, F);
    
    coefficients_ = solution.head(samples_.size());
    tail_ = solution.tail(numberOfDimensions_ + 1);
  }
  
  double RadialBasisFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    double objectiveValue = 0;
      
    arma::uword n = 0;
    for(const auto& sample : samples_) {
      objectiveValue += coefficients_(n) * radialBasisFunction_->getBasisFunctionValue(arma::norm(parameter - sample.first));
      ++n;
    }
    
    return objectiveValue + radialBasisFunction_->getPolynomialTailValue(parameter, tail_);
  }
  
  std::string RadialBasisFunctionModel::toString() const {
    return "radial_basis_function_model";
  }
}
