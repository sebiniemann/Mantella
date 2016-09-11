// Mantella
#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_COUNTER
#include <mantella>

#if !defined(MANTELLA_BUILD_TESTS)
  #error "`MANTELLA_BUILD_TESTS` must be defined to build its test file."
#endif

int main(int argc, char** argv) {
#if defined(MANTELLA_SUPPORT_MPI)
  MPI_Init(&argc, &argv);
#endif

  auto seed = mant::random_seed();
  std::cout << "Seeding random number generators with: " << seed << std::endl;
  mant::seed_random_number_generator(seed);

  try {
    return Catch::Session().run(argc, argv);
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }

#if defined(MANTELLA_SUPPORT_MPI)
  MPI_Finalize();
#endif
}
