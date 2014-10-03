#include <cstdlib>
#include <memory>
#include <iostream>

#include <mpi.h>

#include <hop>

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::SphereFunction(2));
  hop::ParallelStandardParticleSwarmOptimisation2011 parallelOptimisationAlgorithm(optimisationProblem, 1);
  parallelOptimisationAlgorithm.optimise();

  if(parallelOptimisationAlgorithm.getRank() == 0) {
    std::cout << "isFinished: " << parallelOptimisationAlgorithm.isFinished() << std::endl;
    std::cout << "isTerminated: " << parallelOptimisationAlgorithm.isTerminated() << std::endl;
    std::cout << "numberOfIterations: " << parallelOptimisationAlgorithm.getNumberOfIterations() << std::endl;
    std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
    std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
    std::cout << "bestObjectiveValueDelta: " << parallelOptimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
    std::cout << "bestSolution: " << parallelOptimisationAlgorithm.getBestSolution() << std::endl;

    hop::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, 10);
    optimisationAlgorithm.optimise();

    std::cout << "isFinished: " << optimisationAlgorithm.isFinished() << std::endl;
    std::cout << "isTerminated: " << optimisationAlgorithm.isTerminated() << std::endl;
    std::cout << "numberOfIterations: " << optimisationAlgorithm.getNumberOfIterations() << std::endl;
    std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
    std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
    std::cout << "bestObjectiveValueDelta: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
    std::cout << "bestSolution: " << optimisationAlgorithm.getBestSolution() << std::endl;
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
