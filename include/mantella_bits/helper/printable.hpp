namespace mant {
  // Single function helper class to require/provide a string reresentation.
  class Printable {
    public:
      virtual std::string toString() const noexcept = 0;
  };
}
