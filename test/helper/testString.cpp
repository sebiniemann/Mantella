// Catch
#include <catch.hpp>

// Mantella
#include <mantella>

class TestPrintable : public mant::Printable {
  public:
    std::string to_string() const noexcept override {
      return "ThisIsTestPrintable";
    }
};

TEST_CASE("string: to_string(Printable)", "") {
  SECTION("Returns the specified class name.") {
    TestPrintable testPrintable;

    CHECK(mant::to_string(testPrintable) == "ThisIsTestPrintable");
  }
}

TEST_CASE("string: to_string(Printable*)", "") {
  SECTION("Returns the specified class name.") {
    mant::Printable* testPrintable = new TestPrintable;

    CHECK(mant::to_string(testPrintable) == "ThisIsTestPrintable");
  }

  SECTION("Will also work with std::shared_ptr.") {
    std::shared_ptr<mant::Printable> testPrintable = std::shared_ptr<mant::Printable>(new TestPrintable);

    CHECK(mant::to_string(testPrintable) == "ThisIsTestPrintable");
  }
}

TEST_CASE("string: endsWith", "") {
  SECTION("Finds various subsets of endings.") {
    CHECK(mant::endsWith("input.serialised", ".serialised") == true);
    CHECK(mant::endsWith("input.serialised", "d") == true);
    CHECK(mant::endsWith("input.serialised", "put.serialised") == true);
    CHECK(mant::endsWith("input.serialised", "serialised") == true);
  }

  SECTION("Recognised the input itself as an ending.") {
    CHECK(mant::endsWith("input.serialised", "input.serialised") == true);
  }

  SECTION("Ignores substrings of the input, if something from the ending is missing.") {
    CHECK(mant::endsWith("input.serialised", ".serialise") == false);
    CHECK(mant::endsWith("input.serialised", "erialise") == false);
  }

  SECTION("Ignores any string not occuring within the input.") {
    CHECK(mant::endsWith("input.serialised", ".mat") == false);
  }

}



