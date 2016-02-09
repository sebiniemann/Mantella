#include <mantella>

int main() {
  // 1. Setup the optimisation problem.
  unsigned int numberOfDimensions = 2;
  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  // 2. Run the solver.
  mant::HookeJeevesAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.optimise(optimisationProblem);

  // 3. Get your result"
  std::cout << "bestParameter: " << optimisationAlgorithm.getBestParameter() << std::endl;

  return 0;
}
