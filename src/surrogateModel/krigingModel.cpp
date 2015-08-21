#include <mantella_bits/surrogateModel/krigingModel.hpp>

namespace mant {
  KrigingModel::KrigingModel(
      const arma::uword numberOfDimensions)
    : SurrogateModel(numberOfDimensions, false, false) {
    
  }

  void KrigingModel::modelImplementation() {
  
  }
  
  double KrigingModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return 0.0;
  }
  
  std::string KrigingModel::toString() const {
    return "kriging_model";
  }
}