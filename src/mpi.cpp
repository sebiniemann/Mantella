#include "mantella_bits/mpi.hpp"

// C++ standard library
#if defined(SUPPORT_MPI) // IWYU pragma: keep
#include <algorithm>
#include <cassert>
#endif

// Armadillo
#if defined(SUPPORT_MPI)
#include <armadillo> // IWYU pragma: keep
#endif

#if defined(SUPPORT_MPI)
namespace mant {
  void mpiBestSample(
      void* firstInput,
      void* secondInput,
      int* size,
      MPI_Datatype* type) {
    // Casts both inputs to their actual type.
    double* firstParameters = static_cast<double*>(firstInput);
    double* secondParameters = static_cast<double*>(secondInput);

    // Loops through all parameter pairs, as MPI_User_function requires the support of SIMD-like behaviour.
    for (int n = 0; n < *size; ++n) {
      // Corrects the type of the first value from double to *arma::uword*.
      const arma::uword numberOfDimensions = static_cast<unsigned int>(firstParameters[0]);
      assert(numberOfDimensions == static_cast<unsigned int>(secondParameters[0]));

      // MPI uses the second parameter as the result.
      // In case the first parameter was actually better (had a lower objective value), copy all values (except the number of dimensions) to the second one.
      if (firstParameters[1] < secondParameters[1]) {
        std::copy(&firstParameters[1], &firstParameters[2 + numberOfDimensions], &secondParameters[1]);
      }

      // Moves the pointer forward to the next parameter pair.
      firstParameters += 2 + numberOfDimensions;
      secondParameters += 2 + numberOfDimensions;
    }
  }
}
#endif
