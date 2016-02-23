#include <mantella>

int main() {
  ::mant::isVerbose = true;

  mant::bbob::SphereFunction optimisationProblem(2);

  mant::HookeJeevesAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(10); // Just limits the example output.
  optimisationAlgorithm.optimise(optimisationProblem);

  return 0;
}
