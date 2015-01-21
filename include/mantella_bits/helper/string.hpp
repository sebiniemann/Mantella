#pragma once

// C++ Standard Library
#include <memory>

// Mantella
#include <mantella_bits/helper/printable.hpp>

namespace mant {
  // Returns the string representation of a class marked as printable;
  // The underscore within the name is intended, in order to be constistent with Standard Library's
  // std::to_string
  inline std::string to_string(
      // The class marked as printable.
      const Printable& printable) ;
  inline std::string to_string(
      // A shared pointer to the class marked as printable.
      const std::shared_ptr<Printable> printable) ;

  // Checks whether the provided text ends with another string. This is usually used in this library
  // to check/filter for file endings.
  inline bool endsWith(
      // The text to be checked.
      const std::string& text,
      // The suffix searched for.
      const std::string& ending) ;

  //
  // Implementation
  //

  inline std::string to_string(
      const Printable& printable)  {
    return printable.to_string();
  }

  inline std::string to_string(
      const std::shared_ptr<Printable> printable)  {
    return printable->to_string();
  }

  inline bool endsWith(
      const std::string& text,
      const std::string& ending)  {
    if (text.length() >= ending.length()) {
      return (text.compare(text.length() - ending.length(), ending.length(), ending) ==0);
    }

    return false;
  }
}
