#pragma once

#include <memory>

#include <hop_bits/helper/printable.hpp>

namespace hop {
  std::string to_string(const Printable& printable);
  std::string to_string(const std::unique_ptr<Printable> printable);
  std::string to_string(const std::shared_ptr<Printable> printable);

  bool endsWith(const std::string& text, const std::string& ending);
}
