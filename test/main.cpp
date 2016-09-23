// Catch
#define CATCH_CONFIG_COUNTER
#include <catch.hpp>

// Mantella
#define MANTELLA_BUILD_TESTS
#include <mantella>

int main(int argc, char** argv) {
#if defined(MANTELLA_SUPPORT_MPI)
  MPI_Init(&argc, &argv);
#endif

  try {
    return Catch::Session().run(argc, argv);
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }

#if defined(MANTELLA_SUPPORT_MPI)
  MPI_Finalize();
#endif
}