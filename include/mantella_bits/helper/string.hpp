namespace mant {
  inline std::string to_string(
      const Printable& printable) noexcept;

  inline std::string to_string(
      const std::shared_ptr<Printable> printable) noexcept;

  inline bool endsWith(
      const std::string& text,
      const std::string& ending) noexcept;

  //
  // Implementation
  //

  inline std::string to_string(
      const Printable& printable) noexcept {
    return printable.toString();
  }

  inline std::string to_string(
      const std::shared_ptr<Printable> printable) noexcept {
    return printable->toString();
  }

  inline bool endsWith(
      const std::string& text,
      const std::string& ending) noexcept {
    if (text.length() >= ending.length()) {
      return (text.compare(text.length() - ending.length(), ending.length(), ending) == 0);
    }

    return false;
  }
}
