#include "mantella_bits/config.hpp"

// C++ standard library
#if defined(SUPPORT_MPI)
#include <stdexcept> // IWYU pragma: keep
#endif

// MPI
#if defined(SUPPORT_MPI)
#include <mpi.h> // IWYU pragma: keep
#endif

namespace mant {
  bool isCachingSamples(false);
  bool isRecordingSampling(false);
  bool isVerbose(false);
  double machinePrecision(1e-12);

  arma::uword threadNumber() {
#if defined(SUPPORT_OPENMP)
    return omp_get_thread_num();
#else
    return 0;
#endif
  }

  arma::uword numberOfThreads() {
#if defined(SUPPORT_OPENMP)
    return omp_get_max_threads();
#else
    return 1;
#endif
  }

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
