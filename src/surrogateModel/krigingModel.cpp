#include <mantella_bits/surrogateModel/krigingModel.hpp>

namespace mant {
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