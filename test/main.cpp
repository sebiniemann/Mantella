// Catch
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <iostream>
#include <stdexcept>
#include <string>

// Mantella
#include <mantella>

int main(int argc, char* argv[]) {
#if defined(SUPPORT_MPI)
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &nodeRank);
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes);
#endif

  std::ios_base::sync_with_stdio(false);
  // Uncomment the following line to get insights about the optimisation process.
  // ::mant::isVerbose = true;

  // The last argument is used as location for the test data directory.
  // Reduced also the number of arguments, in order avoid conflicts with catch command line arguments handling.
  ::rootTestDataDirectory = argv[--argc];
  std::cout << "Using '" << ::rootTestDataDirectory << "' as location for the root test data directory." << std::endl;

  // Randomly and uniformly initialise the random number generator.
  mant::Rng::setRandomSeed();
  std::cout << "Initialised tests with random seed: " << mant::Rng::getSeed() << std::endl;
  // Setting the seed to a specific number to reproduce erroneous tests.
  // mant::Rng::setSeed(123456789);
  // std::cout << "Restored tests with seed: " << mant::Rng::getSeed() << std::endl;

  try {
    return Catch::Session().run(argc, argv);
  } catch (const std::exception& exception) {
    std::cout << exception.what();
  }

#if defined(SUPPORT_MPI)
  MPI_Finalize();
#endif
}
