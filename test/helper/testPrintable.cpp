// Catch
#include <catch.hpp>

// Mantella
#include <mantella>

class TestPrintable : public mant::Printable {
  public:
    std::string toString() const noexcept override {
      return "ThisIsTestPrintable";
    }
};

TEST_CASE("printable: Printable", "") {
  SECTION("Returns the specified class name.") {
    TestPrintable testPrintable;

    CHECK(testPrintable.toString() == "ThisIsTestPrintable");
  }
}
