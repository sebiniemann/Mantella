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

TEST_CASE("string: to_string(Printable)", "") {
  TestPrintable testPrintable;

  CHECK(mant::to_string(testPrintable) == "ThisIsTestPrintable");
}

TEST_CASE("string: to_string(shared_ptr<Printable>)", "") {
  std::shared_ptr<mant::Printable> testPrintable = std::shared_ptr<mant::Printable>(new TestPrintable);

  CHECK(mant::to_string(testPrintable) == "ThisIsTestPrintable");
}

TEST_CASE("string: endsWith", "") {
  CHECK(mant::endsWith("input.serialised", ".serialised") == true);
  CHECK(mant::endsWith("input.serialised", "put.serialised") == true);
  CHECK(mant::endsWith("input.serialised", "input.serialised") == true);
  CHECK(mant::endsWith("input.serialised", "serialised") == true);
  CHECK(mant::endsWith("input.serialised", ".serialise") == false);
  CHECK(mant::endsWith("input.serialised", "erialise") == false);
  CHECK(mant::endsWith("input.serialised", ".mat") == false);
}



