#include <mantella>

int main() {
  HookeJeevesAlgorithm optimisationAlgorithm;
  std::cout << "Default step size decrease: " << optimisationAlgorithm.getStepSizeDecrease() << std::endl;
  
  optimisationAlgorithm.setStepSizeDecrease(0.1);
  std::cout << "Updated step size decrease: " << optimisationAlgorithm.getStepSizeDecrease() << std::endl;

  return 0;
}
