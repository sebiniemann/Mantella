#include <hop_bits/helper/string.hpp>

namespace hop {
  std::string to_string(
      const Printable& printable) {
    return printable.to_string();
  }

  std::string to_string(
      const std::unique_ptr<Printable> printable) {
    return printable->to_string();
  }

  std::string to_string(
      const std::shared_ptr<Printable> printable) {
    return printable->to_string();
  }

  bool String::endsWith(
      const std::string& text,
      const std::string& ending) {
    if (text.length() >= ending.length()) {
      return (text.compare(text.length() - ending.length(), ending.length(), ending) ==0);
    }

    return false;
  }
}
