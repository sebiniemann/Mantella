#include <mantella>

int main() {
  SimulatedAnnealing optimisationAlgorithm;
  std::cout << "Default maximal step size: " << optimisationAlgorithm.getMaximalStepSize() << std::endl;
  
  optimisationAlgorithm.setMaximalStepSize(0.5);
  std::cout << "Updated maximal step size: " << optimisationAlgorithm.getMaximalStepSize() << std::endl;

  return 0;
}
