namespace mant {
  // Single function helper class to require/provide a string reresentation.
  class Printable {
    public:
      virtual std::string to_string() const  = 0;
  };
}
