// Catch
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "catchHelper.hpp"

int main(int argc, char** argv) {
#if defined(SUPPORT_MPI)
  MPI_Init(&argc, &argv);
#endif

  // Prints out the seed to reproduce the test later on.
  std::cout << "Random seed: " << mant::Rng::setRandomSeed() << std::endl;

  try {
    return Catch::Session().run(argc, argv);
  } catch (const std::exception& exception) {
    std::cout << exception.what();
  }

#if defined(SUPPORT_MPI)
  MPI_Finalize();
#endif
}
