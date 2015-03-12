namespace mant {
  template <typename ParameterType>
  class FunctionModelProperty : public Property<ParameterType> {
    public:
      using Property<ParameterType>::Property;
  };
}
