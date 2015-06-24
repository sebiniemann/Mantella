namespace mant {
  class Printable {
    public:
      virtual std::string toString() const noexcept = 0;
  };
}

inline std::string to_string(
    const std::shared_ptr<Printable> printable) noexcept;

//
// Implementation
//

inline std::string to_string(
    const std::shared_ptr<Printable> printable) noexcept {
  return printable->toString();
}