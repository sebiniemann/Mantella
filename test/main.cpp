// Catch
#define CATCH_CONFIG_COUNTER
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

struct mockup_rng : public std::mt19937_64 {
  static bool use_mockup;
  static std::mt19937_64::result_type last_seed;
  
  void seed(
      std::mt19937_64::result_type seed) {
    std::mt19937_64::seed(seed);
    last_seed = seed;
  }
  
  std::mt19937_64::result_type operator()() {
#if defined(USE_MPI)
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#else
    const unsigned int rank = 0;
#endif

#if defined(USE_OPENMP)
    int thread_number = omp_get_thread_num();
    int number_of_threads = omp_get_max_threads();
#else
    int thread_number = 0;
    int number_of_threads = 1;
#endif

    if (use_mockup) {
      return rank * number_of_threads + thread_number;
    } else {
      return std::mt19937_64::operator()();
    }
  }
};
decltype(mockup_rng::use_mockup) mockup_rng::use_mockup = false;
decltype(mockup_rng::last_seed) mockup_rng::last_seed = 0;

// Mantella
#define MANTELLA_BUILD_TESTS
#define MANTELLA_RANDOM_NUMBER_GENERATOR mockup_rng
#include <mantella0>

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