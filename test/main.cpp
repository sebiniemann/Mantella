// Catch
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

// C++ standard library
#include <string>
#include <stdexcept>
#include <iostream>

// Mantella
#include <mantella>

static std::string testDirectory;

int main(int argc, char* argv[]) {
#if defined(SUPPORT_MPI)
  MPI_Init(&argc, &argv);
#endif

  try {
    if (argc != 2) {
      throw std::invalid_argument(
          "The location of the test directory must be added to the command line.");
    }

    testDirectory = argv[1];

    mant::Rng::setSeed(123456789);

    return Catch::Session().run();
  } catch (const std::exception& exception) {
    std::cout << exception.what();
  }

#if defined(SUPPORT_MPI)
  MPI_Finalize();
#endif
}
