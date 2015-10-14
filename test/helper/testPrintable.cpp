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
    SECTION("Returns a (filesystem friendly) name for the class.") {
      TestPrintable testPrintable;

      CHECK(testPrintable.toString() == "ThisIsTestPrintable");
    }
  }
}

TEST_CASE("to_string") {
  SECTION("Returns a (filesystem friendly) name for the class.") {
    std::shared_ptr<mant::Printable> testPrintable = std::shared_ptr<mant::Printable>(new TestPrintable);

    CHECK(mant::to_string(testPrintable) == "ThisIsTestPrintable");
  }
}
