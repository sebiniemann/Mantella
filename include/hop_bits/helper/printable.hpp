#pragma once

// C++ STL
#include <string>

namespace hop {
  // Single function helper class to require/provide a string reresentation.
  class Printable {
    public:
      virtual std::string to_string() const = 0;
  };
}
