#include <mantella_bits/helper/string.hpp>

namespace mant {
  std::string to_string(
      const Printable& printable) noexcept {
    return printable.to_string();
  }

  std::string to_string(
      const std::unique_ptr<Printable> printable) noexcept {
    return printable->to_string();
  }

  std::string to_string(
      const std::shared_ptr<Printable> printable) noexcept {
    return printable->to_string();
  }

  bool endsWith(
      const std::string& text,
      const std::string& ending) noexcept {
    if (text.length() >= ending.length()) {
      return (text.compare(text.length() - ending.length(), ending.length(), ending) ==0);
    }

    return false;
  }
}
