// Catch
#define CATCH_CONFIG_RUNNER
#include <catch/catch.hpp>

// C++ STL
#include <string>
#include <stdexcept>

// Boost
#include <boost/filesystem.hpp>

boost::filesystem::path testDirectory;

// Test cases
#include <optimisationProblem/benchmark/testBlackBoxOptimisationBenchmark2013.hpp>
#include <helper/testGeometry.hpp>
#include <helper/testString.hpp>

int main(const int argc, const char* argv[]) {
  if(argc != 2) {
    throw std::invalid_argument("The number of arguments (" + std::to_string(argc) + ") provided must be exactly 2.");
  }

  testDirectory = boost::filesystem::path(argv[1]);

  if(!boost::filesystem::exists(testDirectory)) {
    throw std::invalid_argument("The speficied test data directory (" + testDirectory.string() + ") does not exists.");
  }

  return Catch::Session().run();
}
