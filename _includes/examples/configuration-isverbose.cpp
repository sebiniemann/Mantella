#include <mantella>

int main() {
  ::mant::isVerbose = true;

  mant::bbob::SphereFunction optimisationProblem(2);

  mant::HookeJeevesAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.optimise(optimisationProblem);

  return 0;
}
