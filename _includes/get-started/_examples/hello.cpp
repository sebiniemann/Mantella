#include <iostream>
#include <mantella>

int main() {
  // 1. Setup the optimisation problem.
  unsigned int numberOfDimensions = 4;
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new mant::bbob2015::SphereFunction(numberOfDimensions));

  // 2. Run the solver.
  mant::HookeJeevesAlgorithm optimisationAlgorithm(optimisationProblem);
  optimisationAlgorithm.optimise();

  // 3. Get your results!
  std::cout << "isFinished: " << optimisationAlgorithm.isFinished() << std::endl;
  std::cout << "isTerminated: " << optimisationAlgorithm.isTerminated() << std::endl;
  std::cout << "numberOfIterations: " << optimisationAlgorithm.getNumberOfIterations() << std::endl;
  std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
  std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
  std::cout << "bestSoftConstraintsValue: " << optimisationAlgorithm.getBestSoftConstraintsValue() << std::endl;
  std::cout << "bestObjectiveValueDelta: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
  std::cout << "bestParameter: " << optimisationAlgorithm.getBestParameter() << std::endl;

  return 1;
}