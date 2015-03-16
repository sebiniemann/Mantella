namespace mant {
  class HammingDistance : public DistanceFunction<unsigned int> {
    public:
      using DistanceFunction<unsigned int>::DistanceFunction;

    protected:
      inline unsigned int getDistanceImplementation(
          const arma::Col<unsigned int>& parameter) const noexcept override;

      inline arma::Col<unsigned int> getRandomNeighbourImplementation(
          const arma::Col<unsigned int>& parameter,
          const unsigned int minimalDistance,
          const unsigned int maximalDistance) const override;
  };

  //
  // Implementation
  //

  inline unsigned int HammingDistance::getDistanceImplementation(
      const arma::Col<unsigned int>& parameter) const noexcept {
    return arma::accu(parameter != 0);
  }

  inline arma::Col<unsigned int> HammingDistance::getRandomNeighbourImplementation(
      const arma::Col<unsigned int>& parameter,
      const unsigned int minimalDistance,
      const unsigned int maximalDistance) const {
    assert(minimalDistance >= 0);
    assert(minimalDistance <= maximalDistance);

    if(minimalDistance > std::min(getDistanceImplementation(parameter), parameter.n_elem - getDistanceImplementation(parameter))) {
          throw std::logic_error("The minimal distance (" + std::to_string(minimalDistance) + ") must be lower than or equal to the absolute maximal distance (" + std::to_string(std::min(getDistanceImplementation(parameter), parameter.n_elem - getDistanceImplementation(parameter))) + ").");
    }

    arma::Col<unsigned int> flippedParameter = parameter;

    const arma::Col<unsigned int>& elementsToFlip = getRandomPermutation(parameter.n_elem, std::uniform_int_distribution<unsigned int>(minimalDistance, maximalDistance)(Rng::getGenerator()));
    const arma::Col<unsigned int>& flippedElements = parameter.elem(elementsToFlip);

    flippedParameter.elem(flippedElements != 0).fill(0);
    flippedParameter.elem(flippedElements == 0).fill(1);
    return flippedParameter;
  }
}
