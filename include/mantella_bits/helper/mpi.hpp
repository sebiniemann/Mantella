#pragma once
#include "mantella_bits/config.hpp" // IWYU pragma: keep

#if defined(SUPPORT_MPI)
namespace mant {
  void mpiGetBestSample(
      void* firstInput,
      void* secondInput,
      int* size,
      MPI_Datatype* type);
}
#endif
