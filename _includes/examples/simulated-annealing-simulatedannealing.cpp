#include <mantella>

int main() {
  // Initialises the problem
  unsigned int numberOfDimensions = 2;
  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  // Initialises and runs the solver
  mant::SimulatedAnnealing optimisationAlgorithm;
  optimisationAlgorithm.optimise(optimisationProblem);

  // Prints the best (lowest) found objective value as well as its difference to the known optimal one.
  std::cout << "Best objective value: " << optimisationAlgorithm.getBestObjectiveValue() << "\n";
  std::cout << "Difference to the optimal objective value: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem.getOptimalObjectiveValue() << "\n";

  return 0;
}
