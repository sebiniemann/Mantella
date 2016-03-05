#include <mantella>

int main() {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;
  std::cout << "Default maximal local attraction: " << optimisationAlgorithm.getMaximalLocalAttraction() << std::endl;
  
  optimisationAlgorithm.setMaximalLocalAttraction(0.4);
  std::cout << "Updated maximal local attraction: " << optimisationAlgorithm.getMaximalLocalAttraction() << std::endl;

  return 0;
}
