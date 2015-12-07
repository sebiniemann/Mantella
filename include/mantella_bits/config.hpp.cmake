#pragma once

namespace mant {
  // Global variables, to be used in other files.
  extern bool cacheSamples;
  extern bool recordSamples;
  extern bool verboseOutput;
}

// MPI support must be decided via CMake, to ensure that we also link against it.
// Therefore, CMake can decide whether SUPPORT_MPI is to be defined or not.
#cmakedefine SUPPORT_MPI
#if defined(SUPPORT_MPI)
#include <mpi.h>
#endif
