// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <memory>
#include <random>

// Mantella
#include <mantella>

// The effectiveness of any global variable is tested within the test cases for the functions that depend on them.
TEST_CASE("cacheSamples") {
  SECTION("The default value is set to *true*") {
    CHECK(mant::cacheSamples == true);
  }
  
  SECTION("Is changeable") {
    mant::cacheSamples = false;
    CHECK(mant::cacheSamples == false);
  }
}

TEST_CASE("recordSamples") {
  SECTION("The default value is set to *false*") {
    CHECK(mant::recordSamples == false);
  }
  
  SECTION("Is changeable") {
    mant::recordSamples = true;
    CHECK(mant::recordSamples == true);
  }
}
