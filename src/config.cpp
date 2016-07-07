#include "mantella_bits/config.hpp"

// MPI
#if defined(SUPPORT_MPI)
#include <mpi.h> // IWYU pragma: keep
#endif

namespace mant {
  bool isCachingSamples(false);
  bool isRecordingSampling(false);
  bool isVerbose(false);
  double machinePrecision(1e-12);

  arma::uword getNodeRank() {
#if defined(SUPPORT_MPI)
    int nodeRank_;
    MPI_Comm_rank(MPI_COMM_WORLD, &nodeRank_);
    return nodeRank_;
#else
    return 0;
#endif
  }
  arma::uword getNumberOfNode() {
#if defined(SUPPORT_MPI)
    int numberOfNodes_;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes_);
    return numberOfNodes_;
#else
    return 1;
#endif
  }

  arma::uword getThreadNumber() {
#if defined(_OPENMP)
    return omp_get_thread_num();
#else
    return 0;
#endif
  }

  arma::uword getNumberOfThreads() {
#if defined(_OPENMP)
    return omp_get_max_threads();
#else
    return 1;
#endif
  }
}
