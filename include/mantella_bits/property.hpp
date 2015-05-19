namespace mant {
  template <typename T>
  class Property {
    public:
      explicit Property();

      double getPlausibility() const noexcept;

    protected:
      double plausibility_;
  };

  //
  // Implementation
  //

  template <typename T>
  Property<T>::Property()
    : plausibility_(0) {

  }

  template <typename T>
  double Property<T>::getPlausibility() const noexcept {
    return plausibility_;
  }
}
