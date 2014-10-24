#define CATCH_CONFIG_RUNNER
#include <catch/catch.hpp>

#include <string>

std::string testDirectory;

#include <optimisationProblem/benchmark/testBlackBoxOptimisationBenchmark2013.hpp>
#include <helper/testGeometry.hpp>
#include <helper/testString.hpp>

int main(const int argc, const char* argv[]) {
  if(argc < 2) {
    // TODO Add exception
  }

  testDirectory = argv[1];

  return Catch::Session().run();
}
