#pragma once

namespace mant {
  // Global variables, to be used in other files.
  extern bool isCachingSamples;
  extern bool isRecordingSampling;
  extern bool isVerbose;
  extern double machinePrecision;
}

// MPI support must be added via CMake, to ensure that we also link against it.
// Therefore, CMake will decide whether SUPPORT_MPI is to be defined or not.
#cmakedefine SUPPORT_MPI
