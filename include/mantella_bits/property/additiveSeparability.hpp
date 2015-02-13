namespace mant {
  template <typename ParameterType>
  class AdditiveSeparability : public Property<ParameterType> {
    public:
      bool isSeparable() const noexcept;

      void isSeparable(const bool isSeparable) noexcept;

      std::vector<arma::Col<unsigned int>> getSeparation() const noexcept;

      void setSeparation(
          const std::vector<arma::Col<unsigned int>>& separation);

    protected:
      bool isSeparable_;

      std::vector<arma::Col<unsigned int>> separation_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  bool AdditiveSeparability<ParameterType>::isSeparable() const noexcept {
    return isSeparable_;
  }

  // TODO Grant public access to const member files?
  // TODO const bool oder const bool& (memory / compiler optimisation)
  template <typename ParameterType>
  void AdditiveSeparability<ParameterType>::isSeparable(const bool isSeparable) noexcept {
    isSeparable_ = isSeparable;
  }

  template <typename ParameterType>
  std::vector<arma::Col<unsigned int>> AdditiveSeparability<ParameterType>::getSeparation() const noexcept {
    return separation_;
  }

  template <typename ParameterType>
  void AdditiveSeparability<ParameterType>::setSeparation(
      const std::vector<arma::Col<unsigned int>>& separation) {
    separation_ = separation;
  }
}
