#include <mantella>
#include <iostream>

int main() {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new mant::bbob2015::SphereFunction(3));
  
  optimisationProblem.getObjectiveValue({2.3, 1.2, 1.3}); // Actual computation
  optimisationProblem.getObjectiveValue({1.5, 2.6, -0.4}); // Actual computation
  optimisationProblem.getObjectiveValue({2.3, 1.2, 1.3}); // Retrived from cache
  optimisationProblem.getObjectiveValue({2.3, 1.2, 1.3}); // Retrived from cache
  optimisationProblem.getObjectiveValue({0.6, 0.8, 0.0}); // Actual computation
  
  std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
  std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl
  
  return 0;
}