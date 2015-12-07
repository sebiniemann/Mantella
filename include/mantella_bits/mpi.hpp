#pragma once
#include "mantella_bits/config.hpp" // IWYU pragma: keep

#if defined(SUPPORT_MPI)
namespace mant {
  // The interface must be compatible to MPI_User_function. See the MPI documentation for more information.
  void mpiBestSample(
      void* firstInput,
      void* secondInput,
      int* size,
      MPI_Datatype* type);
}
#endif
