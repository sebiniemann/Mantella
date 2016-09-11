#pragma once

inline int number_of_threads();

//
// Implementation
//

inline int number_of_threads() {
#if defined(MANTELLA_SUPPORT_OPENMP)
  return omp_get_max_threads();
#else
  return 1;
#endif
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("number_of_threads", "[config][number_of_threads]") {
#if defined(SUPPORT_OPENMP)
      CHECK(mant::number_of_threads() == omp_get_max_threads());
#else
      CHECK(mant::number_of_threads() == 1);
#endif
}
#endif
