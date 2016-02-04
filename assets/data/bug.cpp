#include <mantella>

int main() {
  ::mant::isVerbose = true;

  mant::bbob::SphereFunction optimisationProblem(2);
  
  mant::HookeJeevesAlgorithm hookeJeevesAlgorithm;
  hookeJeevesAlgorithm.optimise(optimisationProblem);
  
  return 0;
}