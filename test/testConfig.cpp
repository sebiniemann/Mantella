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
    CHECK(mant::isCachingSamples == true);
  }
  
  SECTION("Is changeable") {
    mant::isCachingSamples = false;
    CHECK(mant::isCachingSamples == false);
    // Restores the default value, as it may affect other tests.
    mant::isCachingSamples = true;
  }
}

TEST_CASE("isRecordingSampling") {
  SECTION("The default value is *false*") {
    CHECK(mant::isRecordingSampling == false);
  }
  
  SECTION("Is changeable") {
    mant::isRecordingSampling = true;
    CHECK(mant::isRecordingSampling == true);
    // Restores the default value, as it may affect other tests.
    mant::isRecordingSampling = false;
  }
}

TEST_CASE("verboseOutput") {
  SECTION("The default value is *true*") {
    CHECK(mant::verboseOutput == true);
  }
    
  SECTION("Is changeable") {
    mant::verboseOutput = false;
    CHECK(mant::verboseOutput == false);
  }
}
