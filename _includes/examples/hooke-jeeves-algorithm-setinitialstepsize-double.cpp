#include <mantella>

int main() {
  HookeJeevesAlgorithm optimisationAlgorithm;
  std::cout << "Default initial step size: " << optimisationAlgorithm.getInitialStepSize() << std::endl;
  
  optimisationAlgorithm.setInitialStepSize(1.0);
  std::cout << "Updated initial step size: " << optimisationAlgorithm.getInitialStepSize() << std::endl;

  return 0;
}
