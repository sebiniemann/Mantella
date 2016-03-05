#include <mantella>

int main() {
  HillClimbing optimisationAlgorithm;
  std::cout << "Default minimal step size: " << optimisationAlgorithm.getMinimalStepSize() << std::endl;
  
  optimisationAlgorithm.setMaximalStepSize(1e-3);
  std::cout << "Updated minimal step size: " << optimisationAlgorithm.getMinimalStepSize() << std::endl;

  return 0;
}
