namespace mant {
  template <typename ParameterType>
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

  template <typename ParameterType>
  Property<ParameterType>::Property()
    : plausibility_(0) {

  }

  template <typename ParameterType>
  double Property<ParameterType>::getPlausibility() const noexcept {
    return plausibility_;
  }
}
