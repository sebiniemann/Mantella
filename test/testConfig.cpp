// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

// The effectiveness of each global variable is tested within the test cases for the functions that depend on them.
SCENARIO("isCachingSamples", "[config][isCachingSamples]") {
  GIVEN("The default value is unchanged") {
    THEN("Return false") {
      CHECK(::mant::isCachingSamples == false);
    }
  }

  GIVEN("The default value was changed") {
    WHEN("Set it to true") {
      THEN("Return true") {
        ::mant::isCachingSamples = true;
        CHECK(::mant::isCachingSamples == true);
        ::mant::isCachingSamples = false;
      }
    }
  }
}

SCENARIO("isRecordingSampling", "[config][isRecordingSampling]") {
  GIVEN("The default value is unchanged") {
    THEN("Return false") {
      CHECK(::mant::isRecordingSampling == false);
    }
  }

  GIVEN("The default value was changed") {
    WHEN("Set it to true") {
      THEN("Return true") {
        ::mant::isRecordingSampling = true;
        CHECK(::mant::isRecordingSampling == true);
        ::mant::isRecordingSampling = false;
      }
    }
  }
}

SCENARIO("isVerbose", "[config][isVerbose]") {
  GIVEN("The default value is unchanged") {
    THEN("Return false") {
      CHECK(::mant::isVerbose == false);
    }
  }

  GIVEN("The default value was changed") {
    WHEN("Set it to true") {
      THEN("Return true") {
        ::mant::isVerbose = true;
        CHECK(::mant::isVerbose == true);
        ::mant::isVerbose = false;
      }
    }
  }
}

SCENARIO("SUPPORT_MPI", "[config][SUPPORT_MPI]") {
#if defined(MPI_SUCCESS)
  GIVEN("MPI_SUCCESS is defined") {
    THEN("SUPPORT_MPI is defined") {
#if !defined(SUPPORT_MPI)
      FAIL("SUPPORT_MPI was not defined together with MPI_SUCCESS.");
#endif
    }
  }
#endif

#if !defined(MPI_SUCCESS)
  GIVEN("MPI_SUCCESS not is defined") {
    THEN("SUPPORT_MPI not is defined") {
#if defined(SUPPORT_MPI)
      FAIL("SUPPORT_MPI was not defined together with MPI_SUCCESS.");
#endif
    }
  }
#endif
}
