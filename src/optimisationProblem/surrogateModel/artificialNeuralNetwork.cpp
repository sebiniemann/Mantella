#include "mantella_bits/optimisationProblem/surrogateModel/artificialNeuralNetwork.hpp"

namespace mant {
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
