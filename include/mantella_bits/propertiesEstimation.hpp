namespace mant {
  template <typename ParameterType, class... Properties>
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

  template <typename ParameterType, class... Properties>
  void PropertiesEstimation<ParameterType, Properties...>::estimate(
      const Properties*... properties) noexcept{
    return estimateImplementation(properties...);
  }
}
