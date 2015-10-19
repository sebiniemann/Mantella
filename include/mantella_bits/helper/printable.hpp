#pragma once

// C++ standard library
#include <string>
#include <memory>

namespace mant {
  class Printable {
   public:
    virtual std::string toString() const = 0;

    virtual ~Printable() = default;
  };

  std::string to_string(
      const std::shared_ptr<Printable> printable);
}
