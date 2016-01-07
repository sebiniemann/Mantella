// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <memory>
#include <random>

// Mantella
#include <mantella>

// The effectiveness of any global variable is tested within the test cases for the functions that depend on them.
TEST_CASE("isCachingSamples") {
  SECTION("The default value is *true*") {
    CHECK(::mant::isCachingSamples == false);
  }

  SECTION("Is changeable") {
    ::mant::isCachingSamples = true;
    CHECK(::mant::isCachingSamples == true);
    // Restores the default value, as it may affect other tests.
    ::mant::isCachingSamples = false;
  }
}

TEST_CASE("isRecordingSampling") {
  SECTION("The default value is *false*") {
    CHECK(::mant::isRecordingSampling == false);
  }

  SECTION("Is changeable") {
    ::mant::isRecordingSampling = true;
    CHECK(::mant::isRecordingSampling == true);
    // Restores the default value, as it may affect other tests.
    ::mant::isRecordingSampling = false;
  }
}

TEST_CASE("isVerbose") {
  SECTION("The default value is *true*") {
    CHECK(::mant::isVerbose == false);
  }

  SECTION("Is changeable") {
    ::mant::isVerbose = true;
    CHECK(::mant::isVerbose == true);
    // Restores the default value, as it may affect other tests.
    ::mant::isVerbose = false;
  }
}
