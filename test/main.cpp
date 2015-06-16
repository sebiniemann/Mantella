// Catch
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

// C++ Standard Library
#include <string>
#include <stdexcept>
#include <iostream>

// Mantella
#include <mantella>

std::string testDirectory;

int main(const int argc, const char* argv[]) {
  try {
    if (argc != 2) {
      throw std::invalid_argument("The location of the test directory must be added to the command line.");
    }

    testDirectory = argv[1];

    mant::Rng::setSeed(123456789);

    return Catch::Session().run();
  } catch(const std::exception& exception) {
    std::cout << exception.what();
  }
}
