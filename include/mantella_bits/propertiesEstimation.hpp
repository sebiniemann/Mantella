namespace mant {
  template <typename T, class... Properties>
  class PropertiesEstimation {
    public:
      void estimate(
          const Properties*... properties) noexcept;

      virtual ~PropertiesEstimation() = default;

    protected:
      virtual void estimateImplementation(
          const Properties*... properties) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename T, class... Properties>
  void PropertiesEstimation<T, Properties...>::estimate(
      const Properties*... properties) noexcept{
    return estimateImplementation(properties...);
  }
}
