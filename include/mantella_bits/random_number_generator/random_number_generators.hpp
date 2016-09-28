#pragma once
    
/**
  .. cpp:var:: random_number_generators()
*/
inline random_number_generator_t& random_number_generator();

//
// Implementation
//

inline random_number_generator_t& random_number_generator() {
#if MANTELLA_NUMBER_OF_THREADS > 1
  return random_number_generators[omp_get_thread_num()];
#else
  return random_number_generators[0];
#endif
};

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_number_generator", "[random_number_generator]") {
  mockup_rng::use_mockup = true;
  
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
      
      CHECK(random_number_generator()() == rank * number_of_threads + thread_number);
    }
  }
  mockup_rng::use_mockup = false;
}
#endif