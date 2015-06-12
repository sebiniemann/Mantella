namespace mant {
  template <typename T = double>
  class LinearProportionalityAnalysis : public ActivePropertyAnalysis<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      using ActivePropertyAnalysis<T>::ActivePropertyAnalysis;

      std::string toString() const noexcept override;
      
    protected:
      void analyseImplementation(
          std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept override;
  };
  
  template <typename T>
  std::string LinearProportionalityAnalysis<T>::toString() const noexcept {
    return "linear_proportionality_analysis";
  }
}
