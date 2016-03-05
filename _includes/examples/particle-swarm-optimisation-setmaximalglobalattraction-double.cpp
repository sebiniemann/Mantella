#include <mantella>

int main() {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;
  std::cout << "Default maximal global attraction: " << optimisationAlgorithm.getMaximalGlobalAttraction() << std::endl;
  
  optimisationAlgorithm.setMaximalGlobalAttraction(0.2);
  std::cout << "Updated maximal global attraction: " << optimisationAlgorithm.getMaximalGlobalAttraction() << std::endl;

  return 0;
}
