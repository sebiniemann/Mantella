namespace mant {
  class QuadraticFunctionModelAnalysis : public FunctionModelAnalysis<double> {
    public:
      using FunctionModelAnalysis<double>::FunctionModelAnalysis;

      inline QuadraticFunctionModelProperty getProperty() const noexcept;

    protected:
      QuadraticFunctionModelProperty property_;
  };

  //
  // Implementation
  //

  inline QuadraticFunctionModelProperty QuadraticFunctionModelAnalysis::getProperty() const noexcept {
    return property_;
  }
}
