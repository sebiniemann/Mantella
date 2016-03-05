#include <mantella>

int main() {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;
  optimisationAlgorithm.setNeighbourhoodTopologyFunction(
    [this]() {
       return arma::ones(numberOfParticles_, numberOfParticles_);
    },
    "Global");
  
  std::cout << "Set neighbourhood topology function to : " << optimisationAlgorithm.getNeighbourhoodTopologyFunctionName() << std::endl;
  
  optimisationAlgorithm.setNeighbourhoodTopologyFunction(
    [this]() {
      arma::Mat<arma::uword> neighbourhoodTopology(numberOfParticles_, numberOfParticles_, arma::fill::zeros);
      
      // Sets the offset-th super-diagonal and the (offset - n_rows)-th sub-diagonal to 1 and keeps everything else at 0.
      // For offset = 1, the first particle uses the local best value of the second one, the second of the local best value of the third ones, and so on (see the ASCII art below).
      // *Note:* This way, the particle is not its own neighbours, as the diagonal is still 0. Use `neighbourhoodTopology.diag().ones()` to change this.
      // .x.... (1->2)
      // ..x... (2->3)
      // ...x.. (3->4)
      // ....x. (4->5)
      // .....x (5->6)
      // x..... (6->1)
      arma::uword offset = 1;
      neighbourhoodTopology.diag(offset).ones();
      neighbourhoodTopology.diag(offset - neighbourhoodTopology.n_rows).ones();
      
     return neighbourhoodTopology;
    },
    "Chain");
  
  std::cout << "Set neighbourhood topology function to : " << optimisationAlgorithm.getNeighbourhoodTopologyFunctionName() << std::endl;

  return 0;
}
