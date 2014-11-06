// Catch
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

// C++ Standard Library
#include <string>
#include <stdexcept>
#include <iostream>

// Boost
#include <boost/filesystem.hpp>

// HOP
#include <hop>

boost::filesystem::path testDirectory;

int main(const int argc, const char* argv[]) {
  try {
    if (argc != 2) {
      throw std::invalid_argument("The number of arguments (" + std::to_string(argc) + ") provided must be exactly 2.");
    }

    testDirectory = boost::filesystem::path(argv[1]);

    if (!boost::filesystem::exists(testDirectory)) {
      throw std::invalid_argument("The speficied test data directory (" + testDirectory.string() + ") does not exists.");
    }

    hop::Random::setSeed(1234567890);

    return Catch::Session().run();
  } catch(std::exception exception) {
    std::cout << exception.what() << std::cout;
  }
}
