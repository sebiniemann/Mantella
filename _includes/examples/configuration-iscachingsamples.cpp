#include <mantella>

int main() {
  ::mant::isCachingSamples = true;
  mant::bbob::SphereFunction optimisationProblem(2);
  optimisationProblem.getObjectiveValue({1.0, 2.0}); // First parameter
  optimisationProblem.getObjectiveValue({2.0, 3.0}); // Second parameter
  optimisationProblem.getObjectiveValue({1.0, 2.0}); // First parameter again (retrieved from cache)
  
  std::cout << "Number of evaluations: " << optimisationProblem.getNumberOfEvaluations() << "\n";
  std::cout << "Number of discrete evaluations: " << optimisationProblem.getNumberOfDistinctEvaluations() << "\n";
  std::cout << "Number of cached samples: " << optimisationProblem.getCachedSamples().size() << std::endl;
  
  ::mant::isCachingSamples = false;
  optimisationProblem.reset(); // Clears the cache and resets all counters to 0.
  optimisationProblem.getObjectiveValue({1.0, 2.0}); // First parameter
  optimisationProblem.getObjectiveValue({2.0, 3.0}); // Second parameter
  optimisationProblem.getObjectiveValue({1.0, 2.0}); // First parameter again (recalculated)
  
  std::cout << "Number of evaluations: " << optimisationProblem.getNumberOfEvaluations() << "\n";
  std::cout << "Number of discrete evaluations: " << optimisationProblem.getNumberOfDistinctEvaluations() << "\n";
  std::cout << "Number of cached samples: " << optimisationProblem.getCachedSamples().size() << std::endl;

  return 0;
}
