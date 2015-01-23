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

TEST_CASE("printable: Printable", "") {
  SECTION("Return specified class name") {
    TestPrintable testPrintable;

    CHECK(testPrintable.to_string() == "ThisIsTestPrintable");
  }
}
