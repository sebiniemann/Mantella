#pragma once

#if defined(MANTELLA_USE_MPI)
namespace mant {
  void mpiGetBestParameter(
      void* firstInput,
      void* secondInput,
      int* size,
      MPI_Datatype* type);
}
#endif
