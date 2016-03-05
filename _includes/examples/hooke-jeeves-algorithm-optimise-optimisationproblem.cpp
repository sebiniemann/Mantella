#include <mantella>

int main() {
  unsigned int numberOfDimensions = 2;
  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  mant::HookeJeevesAlgorithm optimisationAlgorithm;
  // Use (-1.0, 0.5) as initial guess. This could also be any random value.
  optimisationAlgorithm.optimise(optimisationProblem, {-1.0, 0.5});

  std::cout << "Best objective value: " << optimisationAlgorithm.getBestObjectiveValue() << "\n";
  std::cout << "Difference to the optimal objective value: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem.getOptimalObjectiveValue() << "\n";

  return 0;
}
