#pragma once

#include <string>

namespace hop {
  class Printable {
    public:
      virtual std::string to_string() const = 0;
  };
}
