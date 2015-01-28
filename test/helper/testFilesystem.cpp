// Catch
#include <catch.hpp>

// Mantella
#include <mantella>

extern std::string testDirectory;

TEST_CASE("filesystem: fileExists(...)", "") {
  SECTION("Returns true if a file exists.") {
    CHECK(mant::fileExists(testDirectory + "/helper/testFilesystem.cpp") == true);
  }

  SECTION("Returns false if a file does not exist.") {
    CHECK(mant::fileExists(testDirectory + "/someNonExistingFile") == false);
  }
}
