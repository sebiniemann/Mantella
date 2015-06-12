namespace mant {
  template <typename T = double>
  class PropertyAnalysis : public Printable {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      virtual ~PropertyAnalysis() = default;
  };
}
