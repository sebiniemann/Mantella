#pragma once

/**

*/
inline void seed(
    const std::random_device::result_type seed);

//
// Implementation
//

inline void seed(
    const std::random_device::result_type seed) {
#if defined(MANTELLA_SUPPORT_MPI)
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#else
  const unsigned int rank = 0;
#endif
      
#pragma omp parallel num_threads(MANTELLA_NUMBER_OF_THREADS)
  {
#pragma omp for schedule(static)
    for (std::size_t n = 0; n < MANTELLA_NUMBER_OF_THREADS; ++n) {
      // Uses a different seed for each generator, deterministically based on the provided one.
      random_number_generator().seed(seed + rank * MANTELLA_NUMBER_OF_THREADS + n);
    }
  }
}

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("seed", "[random_number_generator][seed]") {
  mockup_rng::use_mockup = true;
  
  mant::seed(123456);
  
#if defined(USE_MPI)
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#else
  const unsigned int rank = 0;
#endif

#pragma omp parallel num_threads(MANTELLA_NUMBER_OF_THREADS)
  {
#pragma omp for schedule(static)
    for (std::size_t n = 0; n < MANTELLA_NUMBER_OF_THREADS; ++n) {
#if defined(USE_OPENMP)
      int thread_number = omp_get_thread_num();
      int number_of_threads = omp_get_max_threads();
#else
      int thread_number = 0;
      int number_of_threads = 1;
#endif
      
      CHECK(mockup_rng::last_seed == 123456 + rank * number_of_threads + thread_number);
    }
  }
  mockup_rng::use_mockup = false;
}
#endif
