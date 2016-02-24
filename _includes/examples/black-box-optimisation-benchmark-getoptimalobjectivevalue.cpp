#include <mantella>

int main() {
  unsigned int numberOfDimensions = 2;
  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);
  
  std::cout << optimisationProblem.getOptimalObjectiveValue() << std::endl;
  
  return 0;
}
