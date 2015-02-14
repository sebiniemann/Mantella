namespace mant {
  class LinearFunctionModelAnalysis : public FunctionModelAnalysis<double> {
    public:
      using FunctionModelAnalysis<double>::FunctionModelAnalysis;

      inline LinearFunctionModelProperty getProperty() const noexcept;

    protected:
      LinearFunctionModelProperty property_;
  };

  //
  // Implementation
  //

  inline LinearFunctionModelProperty LinearFunctionModelAnalysis::getProperty() const noexcept {
    return property_;
  }
}
