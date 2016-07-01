// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

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
