#pragma once

// Armadillo
#include <armadillo>

#pragma once

namespace mant {
  inline int threadNumber();
  inline int numberOfThreads();
  
  //
  // Implementation
  //
  
  int threadNumber() {
#if defined(SUPPORT_OPENMP)
    return omp_get_thread_num();
#else
    return 0;
#endif
  }

  int numberOfThreads() {
#if defined(SUPPORT_OPENMP)
    return omp_get_max_threads();
#else
    return 1;
#endif
  }
}


namespace mant {
  int nodeRank();
  int numberOfNodes();
  
  #if defined(SUPPORT_MPI)
    // Declares a custom MPI_User_function (used as custom MPI_Reduce* operation).
    // **Note:** This function will not be available without MPI support, as `MPI_Datatype` is an MPI specific type and needs to be defined.
    // @see https://www.mpi-forum.org/docs/ for more information.
    static void mpiOpBestSample(
        void* firstInput,
        void* secondInput,
        int* size,
        MPI_Datatype* type);
  #endif
  
  template <typename T, std::size_t N>
  std::array<T, N> synchronise(
      std::array<T, N> data);

  template <typename T>
  T synchronise(
      T data);
      
  //
  // Implementation
  //
  
  arma::uword nodeRank() {
#if defined(SUPPORT_MPI)
    int nodeRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &nodeRank);

    if (nodeRank < 0) {
      throw std::runtime_error("getNodeRank: Please check your MPI installation, as we got a negative node rank.");
    }

    return nodeRank;
#else
    return 0;
#endif
  }

  arma::uword numberOfNodes() {
#if defined(SUPPORT_MPI)
    int numberOfNodes;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes);

    if (numberOfNodes < 0) {
      throw std::runtime_error("getNumberOfNode: Please check your MPI installation, as we got a negative number of nodes.");
    }

    return numberOfNodes;
#else
    return 1;
#endif
  }
}
