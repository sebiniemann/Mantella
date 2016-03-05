#include <mantella>

int main() {
  unsigned int numberOfDimensions = 2;
  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  mant::RandomSearch optimisationAlgorithm;
  optimisationAlgorithm.optimise(optimisationProblem);

  std::cout << "Best objective value: " << optimisationAlgorithm.getBestObjectiveValue() << "\n";
  std::cout << "Difference to the optimal objective value: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem.getOptimalObjectiveValue() << "\n";

  return 0;
}
