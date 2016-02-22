#include <mantella>

int main() {
  unsigned int numberOfDimensions = 2;
  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  mant::HookeJeevesAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(10); // Just limits the example output.
  optimisationAlgorithm.optimise(optimisationProblem);

  std::cout << "bestParameter: " << optimisationAlgorithm.getBestParameter() << std::endl;

  return 0;
}
