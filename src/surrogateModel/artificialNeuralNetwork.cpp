#include <mantella_bits/surrogateModel/artificialNeuralNetwork.hpp>

namespace mant {
  ArtificialNeuralNetwork::ArtificialNeuralNetwork(
      const arma::uword numberOfDimensions)
    : SurrogateModel(numberOfDimensions, false, false) {
    
  }

  void ArtificialNeuralNetwork::modelImplementation() {
  
  }
  
  double ArtificialNeuralNetwork::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return 0.0;
  }
  
  std::string ArtificialNeuralNetwork::toString() const {
    return "artificial_neural_network";
  }
}