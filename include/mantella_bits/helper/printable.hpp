namespace mant {
  class Printable {
    public:
      virtual std::string toString() const noexcept = 0;
  };
}
