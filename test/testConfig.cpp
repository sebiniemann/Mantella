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
    mant::cacheSamples = false;
    CHECK(mant::cacheSamples == false);
  }
}

TEST_CASE("recordSamplingHistory") {
  SECTION("The default value is *false*") {
    CHECK(mant::recordSamplingHistory == false);
  }
  
  SECTION("Is changeable") {
    mant::recordSamplingHistory = true;
    CHECK(mant::recordSamplingHistory == true);
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
