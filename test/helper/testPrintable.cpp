// Catch
#include <catch.hpp>

// Mantella
#include <mantella>

class TestPrintable : public mant::Printable {
  public:
    std::string to_string() const  override {
      return "ThisIsTestPrintable";
    }
};

TEST_CASE("printable: Printable", "") {
  TestPrintable testPrintable;

  CHECK(testPrintable.to_string() == "ThisIsTestPrintable");
}
