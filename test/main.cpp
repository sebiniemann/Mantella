// Catch
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

// C++ Standard Library
#include <string>
#include <stdexcept>
#include <iostream>

// Boost
#include <boost/filesystem.hpp>

// Mantella
#include <mantella>

boost::filesystem::path testDirectory;

int main(const int argc, const char* argv[]) {
  try {
    if (argc != 2) {
      throw std::invalid_argument("The location of the test data directory need to be added to the command line.");
    }

    testDirectory = boost::filesystem::path(argv[1]);

    if (!boost::filesystem::exists(testDirectory)) {
      throw std::invalid_argument("The speficied test data directory (" + testDirectory.string() + ") does not exists.");
    }

    mant::Rng::setSeed(1234567890);

    return Catch::Session().run();
  } catch(const std::exception& exception) {
    std::cout << exception.what();
  }
}
