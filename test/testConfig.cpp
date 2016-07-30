// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("SUPPORT_MPI", "[config][SUPPORT_MPI]") {
#if defined(MPI_SUCCESS)
  WHEN("MPI_SUCCESS is defined") {
    THEN("SUPPORT_MPI is defined") {
#if !defined(SUPPORT_MPI)
      FAIL("SUPPORT_MPI was not defined together with MPI_SUCCESS.");
#endif
    }
  }
#endif

#if !defined(MPI_SUCCESS)
  WHEN("MPI_SUCCESS not is defined") {
    THEN("SUPPORT_MPI not is defined") {
#if defined(SUPPORT_MPI)
      FAIL("SUPPORT_MPI was not defined together with MPI_SUCCESS.");
#endif
    }
  }
#endif
}

SCENARIO("SUPPORT_OPENMP", "[config][SUPPORT_OPENMP]") {
#if defined(_OPENMP)
  WHEN("_OPENMP is defined") {
    THEN("SUPPORT_OPENMP is defined") {
#if !defined(SUPPORT_OPENMP)
      FAIL("SUPPORT_OPENMP was not defined together with _OPENMP.");
#endif
    }
  }
#endif

#if !defined(_OPENMP)
  WHEN("_OPENMP not is defined") {
    THEN("SUPPORT_OPENMP not is defined") {
#if defined(SUPPORT_OPENMP)
      FAIL("SUPPORT_OPENMP was not defined together with _OPENMP.");
#endif
    }
  }
#endif
}

SCENARIO("isCachingSamples", "[config][isCachingSamples]") {
  WHEN("The default value is unchanged") {
    THEN("Return false") {
      CHECK(::mant::isCachingSamples == false);
    }
  }
}

SCENARIO("isRecordingSampling", "[config][isRecordingSampling]") {
  WHEN("The default value is unchanged") {
    THEN("Return false") {
      CHECK(::mant::isRecordingSampling == false);
    }
  }
}

SCENARIO("isVerbose", "[config][isVerbose]") {
  WHEN("The default value is unchanged") {
    THEN("Return false") {
      CHECK(::mant::isVerbose == false);
    }
  }
}

SCENARIO("machinePrecision", "[config][machinePrecision]") {
  WHEN("The default value is unchanged") {
    THEN("Return 1e-12") {
      CHECK(::mant::machinePrecision == Approx(1e-12));
    }
  }
}

SCENARIO("threadNumber", "[config][threadNumber]") {
#if defined(SUPPORT_OPENMP)
  WHEN("OpenMP is supported") {
    THEN("Contains the thread number") {
      CHECK(mant::threadNumber() == static_cast<arma::uword>(omp_get_thread_num()));
    }
  }
#else
  WHEN("OpenMP is not supported") {
    THEN("Contains 0") {
      CHECK(mant::threadNumber() == 0);
    }
  }
#endif
}

SCENARIO("numberOfThreads", "[config][numberOfThreads]") {
#if defined(SUPPORT_OPENMP)
  WHEN("OpenMP is supported") {
    THEN("Contains the number of threads") {
      CHECK(mant::numberOfThreads() == static_cast<arma::uword>(omp_get_max_threads()));
    }
  }
#else
  WHEN("OpenMP is not supported") {
    THEN("Contains 1") {
      CHECK(mant::numberOfThreads() == 1);
    }
  }
#endif
}

SCENARIO("nodeRank", "[config][nodeRank]") {
#if defined(SUPPORT_MPI)
  WHEN("MPI is supported") {
    THEN("Contains the node rank") {
      int nodeRank = 0;
      MPI_Comm_rank(MPI_COMM_WORLD, &nodeRank);
      CHECK(mant::nodeRank() == static_cast<arma::uword>(nodeRank));
    }
  }
#else
  WHEN("MPI is not supported") {
    THEN("Contains 0") {
      CHECK(mant::nodeRank() == 0);
    }
  }
#endif
}

SCENARIO("numberOfNodes", "[config][numberOfNodes]") {
#if defined(SUPPORT_MPI)
  WHEN("MPI is supported") {
    THEN("Contains the number of nodes") {
      int numberOfNodes = 0;
      MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes);
      CHECK(mant::numberOfNodes() == static_cast<arma::uword>(numberOfNodes));
    }
  }
#else
  WHEN("MPI is not supported") {
    THEN("Contains 1") {
      CHECK(mant::numberOfNodes() == 1);
    }
  }
#endif
}
