#include <mantella_bits/surrogateModel/gaussianMixtureModel.hpp>

namespace mant {
  GaussianMixtureModel::GaussianMixtureModel(
      const arma::uword numberOfDimensions)
    : SurrogateModel(numberOfDimensions, false, false) {
    
  }

  void GaussianMixtureModel::modelImplementation() {
  
  }
  
  double GaussianMixtureModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return 0.0;
  }
  
  std::string GaussianMixtureModel::toString() const {
    return "gaussian_mixture_model";
  }
}