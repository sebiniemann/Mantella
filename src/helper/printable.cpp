#include "mantella_bits/helper/printable.hpp"

namespace mant {
  std::string to_string(
      const std::shared_ptr<Printable> printable) {
    return printable->toString();
  }
}
