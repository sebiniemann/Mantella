#pragma once

inline int thread_number();

//
// Implementation
//

inline int thread_number() {
#if defined(MANTELLA_SUPPORT_OPENMP)
  return omp_get_thread_num();
#else
  return 0;
#endif
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("thread_number", "[thread_number]") {
#if defined(SUPPORT_OPENMP)
  CHECK(mant::thread_number() == omp_get_thread_num());
#else
  CHECK(mant::thread_number() == 0);
#endif
}
#endif