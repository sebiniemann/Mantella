#include <mantella_bits/helper/mpi.hpp>

// C++ standard library
#include <algorithm>

// Armadillo
#include <armadillo>

#if defined(SUPPORT_MPI)
namespace mant {
  void mpiGetBestSample(
      void* firstInput,
      void* secondInput,
      int* size,
      MPI_Datatype* type) {
    double* firstParameters = static_cast<double*>(firstInput);
    double* secondParameters = static_cast<double*>(secondInput);

    for (int n = 0; n < *size; ++n) {
      const arma::uword numberOfDimensions = static_cast<unsigned int>(firstParameters[0]);

      if (firstParameters[1] < secondParameters[1]) {
        std::copy(&firstParameters[1], &firstParameters[1 + numberOfDimensions], &secondParameters[1]);
      }

      firstParameters += 2 + numberOfDimensions;
      secondParameters += 2 + numberOfDimensions;
    }
  }
}
#endif
