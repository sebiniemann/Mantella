#pragma once

// C++ standard library
#include <atomic>

namespace mant { 
  extern std::atomic<bool> cacheSamples; /* = true (default) */
  extern std::atomic<bool> recordSamples; /* = false (default) */
}

#undef MANTELLA_USE_MPI
#if defined(MPI_SUCCESS)
  #define MANTELLA_USE_MPI
#endif
