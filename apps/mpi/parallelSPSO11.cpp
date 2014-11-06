#include <cstdlib>
#include <memory>
#include <iostream>
#include <chrono>

#include <mpi.h>

#include <hop>

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  arma::arma_rng::set_seed_random();
  hop::Random::setSeed(arma::randi<arma::Col<arma::uword>>(rank + 1).at(rank));

  double localAttraction = 2.0;
  double globalAttraction = 2.0;
  double acceleration = 0.0;
  double neighbourhoodProbability = 0.55;
  unsigned int communicationSteps = 10;
  unsigned int localPopulationSize = 1;

  unsigned int numberOfIterations = 15;
  unsigned int numberOfDimensions = 4;
  unsigned int maximalNumberOfIterations = 10000;

  arma::Col<double> duration(numberOfIterations);
  arma::Col<arma::uword> finished(numberOfIterations);
  for (unsigned int n = 0; n < numberOfIterations; ++n) {
    std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::bbob2013::SphereFunction(numberOfDimensions));

    hop::ParallelStandardParticleSwarmOptimisation2011 parallelOptimisationAlgorithm(optimisationProblem, localPopulationSize);
    parallelOptimisationAlgorithm.setLocalAttraction(localAttraction);
    parallelOptimisationAlgorithm.setGlobalAttraction(globalAttraction);
    parallelOptimisationAlgorithm.setAcceleration(acceleration);
    parallelOptimisationAlgorithm.setNeighbourhoodProbability(neighbourhoodProbability);
    parallelOptimisationAlgorithm.setCommunicationSteps(communicationSteps);
    parallelOptimisationAlgorithm.setMaximalNumberOfIterations(maximalNumberOfIterations);

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    parallelOptimisationAlgorithm.optimise();
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    duration.at(n) = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    finished.at(n) = parallelOptimisationAlgorithm.isFinished();
  }

  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    if (any(finished)) {
      arma::Col<double>::fixed<1> result = {arma::median(duration.elem(arma::find(finished)))};
      result.save("./duration_parallel_10_" + std::to_string(size) + ".mat", arma::raw_ascii);
    }
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
