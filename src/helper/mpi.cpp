#include <mantella_bits/helper/mpi.hpp>

// C++ standard library
#include <algorithm>

#if defined(MANTELLA_USE_MPI)
namespace mant {
  void mpiGetBestParameter(
      void* firstInput,
      void* secondInput,
      int* size,
      MPI_Datatype* type) {
    double* firstParameters = static_cast<double*>(firstInput);
    double* secondParameters = static_cast<double*>(secondInput);
  
    if(firstParameters[1] < secondParameters[1]) {
      std::copy(&firstParameters[1], &firstParameters[1 + static_cast<unsigned int>(secondParameters[0])], &secondParameters[1]);
    }
  }
}
#endif