#include <mantella>

int main() {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;
  std::cout << "Default maximal acceleration: " << optimisationAlgorithm.getMaximalAcceleration() << std::endl;
  
  optimisationAlgorithm.setMaximalAcceleration(0.5);
  std::cout << "Updated maximal acceleration: " << optimisationAlgorithm.getMaximalAcceleration() << std::endl;

  return 0;
}
