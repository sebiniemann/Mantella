// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// Mantella
#include <mantella>

class TestPrintable : public mant::Printable {
  public:
    std::string toString() const override {
      return "ThisIsTestPrintable";
    }
};

TEST_CASE("Printable") {
  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      TestPrintable testPrintable;

      CHECK(testPrintable.toString() == "ThisIsTestPrintable");
    }
  }
}

TEST_CASE("to_string") {
  SECTION("Returns the expected class name.") {
    std::shared_ptr<mant::Printable> testPrintable = std::shared_ptr<mant::Printable>(new TestPrintable);

    CHECK(mant::to_string(testPrintable) == "ThisIsTestPrintable");
  }
}



