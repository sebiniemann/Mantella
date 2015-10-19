#pragma once

// C++ standard library
#include <atomic>

namespace mant {
  extern std::atomic<bool> cacheSamples;
  extern std::atomic<bool> recordSamples;
}

// MPI
#cmakedefine SUPPORT_MPI
#if defined(SUPPORT_MPI)
#include <mpi.h>
#endif
